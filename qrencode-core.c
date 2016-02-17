/*
  Copyright (C) 2016 by Syohei YOSHIDA

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emacs-module.h>
#include <qrencode.h>

int plugin_is_GPL_compatible;

static emacs_value
Fqrencode_encode(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	size_t scale = env->extract_integer(env, args[1]);

	ptrdiff_t len = 0;
	env->copy_string_contents(env, args[0], NULL, &len);
	char *str_buf = malloc(len);
	env->copy_string_contents(env, args[0], str_buf, &len);

	QRcode *qrcode = QRcode_encodeString(str_buf, 0, QR_ECLEVEL_L,
					     QR_MODE_8, 1);

	int width = (qrcode->width * scale);

	char *buf = (char*)malloc(sizeof(char)*width*width+256);
	char *orig = buf;
	int written = sprintf(buf, "P1\n%d %d\n", width, width);
	buf += written;

	unsigned char *d = qrcode->data;
	for (size_t y = 0; y < width; y+=scale) {
		size_t y_index = y * width;
		for (size_t x = 0; x < width; x+=scale) {
			char v = (*d & 1) ? '1' : '0';
			for (size_t iy = 0; iy < scale; ++iy) {
				size_t iy_index = (y_index+iy*width);
				for (size_t ix = 0; ix < scale; ++ix) {
					buf[iy_index+(x+ix)] = v;
				}
			}
			++d;
		}
	}

	emacs_value str = env->make_string(env, orig, (ptrdiff_t)(written + (width*width)));
	free(orig);
	QRcode_free(qrcode);

	return str;
}

static void
bind_function(emacs_env *env, const char *name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };

	env->funcall(env, Qfset, 2, args);
}

static void
provide(emacs_env *env, const char *feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern (env, "provide");
	emacs_value args[] = { Qfeat };

	env->funcall(env, Qprovide, 1, args);
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

#define DEFUN(lsym, csym, amin, amax, doc, data) \
	bind_function(env, lsym, env->make_function(env, amin, amax, csym, doc, data))

	DEFUN("qrencode-encode", Fqrencode_encode, 2, 2, "Encode string to QRCode", NULL);

#undef DEFUN

	provide(env, "qrencode-core");
	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
