# 02-apply-project-runtime-fix: Apply Project Runtime Fix (if needed)

- Unload project, edit .vcxproj to align RuntimeLibrary flag (/MDd), validate with cppupgrade_validate_vcxproj_file, then reload project.
- Build incrementally and verify link warning cleared.
- Commit changes (After Each Task).
