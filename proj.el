(defun tanks/get-build-path ()
  "Return the path where cmake is to build from."
  (concat "build/" (system-name))
  )

;; set the default directory
(setq cap/source_root_dir default-directory)
(setq cap/build-dir (concat cap/source_root_dir (tanks/get-build-path)))
(setq cap/bin-dir (concat cap/build-dir "/bin"))
(setq cap/install-dir (concat cap/build-dir "/install"))
(setq cap/ld-library-path (concat "LD_LIBRARY_PATH=" cap/install-dir))

;; create initial build dir
(if (equal (file-directory-p cap/build-dir) nil)
    (progn
      (make-directory cap/build-dir)
      (let ((default-directory cap/build-dir)) (shell-command (concat "conan install " cap/source_root_dir " --build missing")))
      (cap/cmake)
      ))

(setq c-default-style "my-cc-style")

(load-file "capengine/init.el")

(defun tanks/launch-nodebug ()
  (interactive)
  (progn
    ;;(cap/cmake-install)
    (async-shell-command
     (concat "cd " cap/bin-dir " && " cap/ld-library-path " gdb -q -batch -ex 'run' -ex 'thread apply all backtrace' --args tanks") "*tanks-run*")))

(defun tanks/launch-debug ()
  (interactive)
  (progn
    ;;(cap/cmake-install)
    (setenv "LD_LIBRARY_PATH" cap/install-dir)
    (gdb (concat "gdb -i=mi --cd " cap/bin-dir " --args tanks"))
    ))

(defun tanks/valgrind ()
  (interactive)
  (progn
    (let ((default-directory cap/bin-dir))
      (progn
	(setenv "LD_LIBRARY_PATH" cap/install-dir)
	(async-shell-command (concat "valgrind --tool=memcheck --leak-check=full ./tanks") "*tanks-valgrind*")
    ))))


(global-set-key (kbd "C-<f5>") 'tanks/launch-nodebug)
(global-set-key (kbd "<f5>") 'tanks/launch-debug)
(global-set-key (kbd "C-S-<f5>") 'gdb-restore-windows)


(tanks/get-build-path)
