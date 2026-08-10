# Plan: Fix C++ Build Issues after Build Tools Upgrade

Scope: Project at Q:\MDS\Y2S1\3Dgraphics\GD1P04-OpenGL\OpenGL_Project\OpenGL_Project.sln

Confirmed flow: Automatic
Working branch: CppBuildToolsUpgrade-fix (will be created from Assignment4 when execution begins)
Commit strategy: After Each Task

Objective: Resolve the In-scope linker warning (LNK4098) and optionally the three C4xxx code warnings. Provide a safe, ordered task list that preserves build stability and uses Visual Studio project edits + small code fixes.

Options for execution (choose one before I start):

- Option A (Recommended): Fix only the In-scope issue (LNK4098). Steps:
  1. Investigate project Runtime Library settings (/MD vs /MT) in the .vcxproj.
  2. If mismatch found, change project to use /MDd (Multi-threaded DLL debug) to match linked libs, or add /NODEFAULTLIB for MSVCRT only if appropriate.
  3. Reload project and build incrementally (cppupgrade_build_and_get_issues).
  4. Final full rebuild to validate all issues resolved.

- Option B: Fix LNK4098 and all C4xxx warnings in this run. Steps:
  - All steps from Option A plus:
  5. Fix C4018 in LightManager.cpp by making index comparisons type-consistent (use int for pointLightCount or use size_t for indices and adapt code accordingly).
  6. Fix C4244 in main.cpp by casting WindowWidth/WindowHeight to float where passed to SetOrtho or add an overload accepting ints.
  7. Fix C4244 in Mesh.cpp by ensuring float flipY = static_cast<float>(windowHeight - _ypos) or compute as float.
  8. Build incrementally after each fix and perform final full rebuild.

Task breakdown (ordered by dependency):

1. Task 01 - Investigate Project Runtime Library
- Read .vcxproj file to confirm RuntimeLibrary settings and linked library list.
- Output: tasks/01-investigate-runtime/task.md (investigation notes)

2. Task 02 - Apply Project Runtime Fix (if needed)
- Unload project, edit .vcxproj to align RuntimeLibrary flag (/MDd), validate with cppupgrade_validate_vcxproj_file, then reload project.
- Build incrementally and verify link warning cleared.
- Commit changes (After Each Task).

3. Task 03 - Code fixes (optional, 3 subtasks)
- 03.01 LightManager C4018: adjust types or cast
- 03.02 main.cpp C4244: explicit casts for SetOrtho arguments
- 03.03 Mesh.cpp C4244: cast _ypos or use float arithmetic
- Build after each subtask; commit after each.

4. Task 04 - Final full rebuild and verification
- Run cppupgrade_rebuild_and_get_issues to confirm 0 warnings and 0 errors.

Risks & Notes:
- Changing Runtime Library may require rebuilding or replacing third-party libs (glfw3.lib, glew32.lib). If they were built with a different CRT, you may need matching builds. If third-party libs are incompatible, we will surface that and ask how you want to proceed.
- I will not commit to your current branch; I will create a new branch `CppBuildToolsUpgrade-fix` from `Assignment4` and work there. I will not push changes without your instruction.

Next step:
- Confirm which Option to run (Option A or Option B). After you confirm, I will start Execution preparation: create the working branch and start Task 01.

Plan generated at: (workflow)
