(defconst ivan-style
 '((c++-offsets-alist . ((substatement-open . 0)
			(label . /)
			(access-label . /)
			(case-label . *)
			(statement-case-intro . *)
			(brace-list-open . 0)
			(statement-cont . c-lineup-math)
			(inline-open . 0)
			(member-init-intro . 0)
			(arglist-close . 0)
			(statement-case-open . *))))
 "The Standard IVAN Programming Style")

(add-hook 'c++-mode-common-hook 'ivan-mode-common-hook)

(defun ivan-mode-common-hook ()
 ;; add style and set it for the current buffer
 (c++-add-style "IVAN" ivan-style t))

