(defconst ivan-c-style
  '((c-offsets-alist            . ((arglist-close . c-lineup-arglist)
                                   (substatement-open . 0)
                                   (case-label        . 4)
                                   (block-open        . 0)
                                   (knr-argdecl-intro . -))))
  "Ivan C++ Programming Style")

;; offset customizations not in my-c-style
(setq c-offsets-alist '((member-init-intro . ++)))

;; Customizations for all modes in CC Mode.
(defun ivan-c-mode-common-hook ()
  ;; add my personal style and set it for the current buffer
  (c-add-style "IVAN" ivan-c-style t))

(add-hook 'c-mode-common-hook 'my-c-mode-common-hook)

