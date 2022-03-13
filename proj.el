(defun tanks/get-build-path ()
  "Return the path where cmake is to build from."
  (concat "build/" (system-name))
  )


;; set the default directory
;;(setq cap/source_root_dir default-directory)
;; (setq cap/source_root_dir (file-name-directory buffer-file-name))
(setq cap/source_root_dir (cap/find-dir-locals-folder))

(setq cap/build-dir (concat cap/source_root_dir (tanks/get-build-path)))
(setq cap/bin-dir (concat cap/build-dir "/bin"))
(setq cap/install-dir (concat cap/build-dir "/install"))
(setq cap/ld-library-path (concat "LD_LIBRARY_PATH=" cap/install-dir))

(setq projectile-project-root cap/source_root_dir)
(setq projectile-git-command "cat <(git ls-files -zco --exclude-standard) <(git --no-pager submodule --quiet foreach 'git ls-files --full-name -co --exclude-standard | sed s!^!$path/!')")

;; create initial build dir
(if (equal (file-directory-p cap/build-dir) nil)
    (progn
      (make-directory cap/build-dir)
      (let ((default-directory cap/build-dir)) (shell-command (concat "conan install " cap/source_root_dir " --build missing")))
      (cap/cmake)
      ))

(setq c-default-style "my-cc-style")

(load-file (concat (cap/find-dir-locals-folder) "capengine/init.el"))

(defun tanks/launch-nodebug ()
  (interactive)
  (progn
    (shell-command (concat "cmake --build " cap/build-dir " --target copy"))
    (let ((default-directory (concat cap/build-dir "/bin")))
	  (async-shell-command "gdb -q -batch -ex 'run' -ex 'thread apply all backtrace' --args tanks") "*tanks-run*")))

(defun tanks/launch-debug ()
  (interactive)
  (progn
    (shell-command (concat "cmake --build " cap/build-dir " --target copy"))
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

(dap-register-debug-template
  "GDB::Run tanks"
  (list :type "gdb"
	:request "launch"
	:name "LLDB::Run"
	:target "tanks"
	:cwd (concat cap/build-dir "/bin")
	;;:env '(("PATH" . (concat  default-directory "/" (tanks/get-build-path) "/bin")))
	))

(tanks/get-build-path)
