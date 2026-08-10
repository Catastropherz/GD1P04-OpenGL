# 03.02-fix-code-warnings: Fix code warnings C4018 and C4244

## 03.02-fix-code-warnings: Fix C4018 and C4244 warnings

Objective:
- Apply minimal, safe code changes to remove the three warnings reported: LightManager.cpp (C4018), main.cpp (C4244 for SetOrtho), Mesh.cpp (C4244 for flipY).

Steps:
1. Edit LightManager.cpp to make index/pointLightCount types consistent (use int or size_t as appropriate) or cast to suppress signed/unsigned mismatch safely.
2. In main.cpp, change SetOrtho(WindowWidth, WindowHeight) to call with floats: SetOrtho(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight)) or adjust overloads.
3. In Mesh.cpp, ensure float flipY = static_cast<float>(windowHeight - _ypos) or cast _ypos to float before subtracting.
4. Build after each edit and ensure no new warnings introduced.

Done-when:
- The three warnings no longer appear for the modified projects after incremental builds.
