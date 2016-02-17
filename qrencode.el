;;; qrencode.el --- libqrencode binding

;; Copyright (C) 2016 by Syohei YOSHIDA

;; Author: Syohei YOSHIDA <syohex@gmail.com>
;; URL: https://github.com/syohex/
;; Version: 0.01

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:

;; libqrencode(https://fukuchi.org/works/qrencode/) binding of Emacs Lisp.

;;; Code:

(require 'qrencode-core)

;;;###autoload
(defun qrencode-region (beg end)
  (interactive "r")
  (qrencode (buffer-substring-no-properties beg end)))

;;;###autoload
(defun qrencode (str &optional scale)
  (interactive
   (list (read-string "> ")))
  (let ((qrcode (qrencode-encode str (or scale 10))))
    (with-current-buffer (get-buffer-create "*qrencode*")
      (read-only-mode -1)
      (erase-buffer)
      (insert qrcode)
      (goto-char (point-min))
      (image-mode)
      (read-only-mode +1)
      (pop-to-buffer (current-buffer)))))

(provide 'qrencode)

;;; qrencode.el ends here
