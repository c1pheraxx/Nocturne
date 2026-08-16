<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0d1117&height=200&section=header&text=Nocturne&fontSize=70&fontColor=8b5cf6&animation=fadeIn&fontAlignY=35&desc=External%20Memory%20Toolkit%20for%20Unity%20Games&descAlignY=55&descSize=18"/>
</p>

<p align="center">
  <a href="https://github.com/c1pheraxx/Nocturne/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-8b5cf6?style=for-the-badge&logo=opensourceinitiative&logoColor=white"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/Architecture-x64-2ea043?style=for-the-badge&logo=amd&logoColor=white"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/Unity-2020%2B-000000?style=for-the-badge&logo=unity&logoColor=white"/>
  </a>
  <br><br>
  <a href="#">
    <img src="https://img.shields.io/badge/External-Read--Only-ff6b6b?style=for-the-badge"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/No%20Injection-No%20DLLs-51cf66?style=for-the-badge"/>
  </a>
  <a href="#">
    <img src="https://img.shields.io/badge/Status-v0.1.0%20Alpha-fb923c?style=for-the-badge"/>
  </a>
</p>

<p align="center">
  <b>Read Unity games from the outside.</b><br>
  No injection. No DLLs. No traces.
</p>

<br>

<h2>Table of Contents</h2>

<ul>
  <li><a href="#overview">Overview</a></li>
  <li><a href="#features">Features</a></li>
  <li><a href="#architecture">Architecture</a></li>
  <li><a href="#quick-start">Quick Start</a></li>
  <li><a href="#build">Build</a></li>
  <li><a href="#configuration">Configuration</a></li>
  <li><a href="#troubleshooting">Troubleshooting</a></li>
  <li><a href="#faq">FAQ</a></li>
  <li><a href="#roadmap">Roadmap</a></li>
  <li><a href="#contributing">Contributing</a></li>
  <li><a href="#contributors">Contributors</a></li>
  <li><a href="#license">License</a></li>
</ul>

<br>

<h2 id="overview">Overview</h2>

<p>Every Unity game reinvents the same wheel internally. Transform hierarchies, camera matrices, bone chains, object registries. The patterns are identical, but the offsets change. Nocturne exists because writing raw <code>ReadProcessMemory</code> calls and hand-rolling AOB scans for the hundredth time is a waste of energy.</p>

<p>This is a clean, modular C++ toolkit that attaches to a Unity process, resolves its internal structures, and hands you the data: positions, rotations, skeletons, camera view matrices, world-to-screen projection. You bring the offsets for your target. Nocturne handles the rest.</p>

<pre>
┌────────────────────────────────────────┐
│  Your Tool (ESP, Aimbot, Radar, etc.)  │
├────────────────────────────────────────┤
│  Registry (EntityScanner)              │
├────────────────────────────────────────┤
│  Unity Readers (Resolver, Transform,   │
│  Camera, Skeleton)                     │
├────────────────────────────────────────┤
│  Core (Process, Memory, Module,       │
│  Pattern)                              │
├────────────────────────────────────────┤
│  Windows API                           │
└────────────────────────────────────────┘
</pre>

<br>

<h2 id="features">Features</h2>

<table>
  <tr><th>Feature</th><th>Description</th></tr>
  <tr><td><b>Process Attachment</b></td><td>Attach by name or PID via standard Windows APIs. No injection required.</td></tr>
  <tr><td><b>Memory Reading</b></td><td><code>ReadProcessMemory</code> wrapper with typed reads, pointer chains, and string reads.</td></tr>
  <tr><td><b>Pattern Scanning</b></td><td>AOB scan with wildcards (<code>??</code>) across module memory space.</td></tr>
  <tr><td><b>Transform Reader</b></td><td>Position, rotation, scale from <code>UnityEngine.Transform</code>.</td></tr>
  <tr><td><b>Camera Reader</b></td><td>View matrix, FOV, near/far clip planes from <code>UnityEngine.Camera</code>.</td></tr>
  <tr><td><b>Skeleton Enumeration</b></td><td>Recursive bone hierarchy traversal from root transforms.</td></tr>
  <tr><td><b>World-to-Screen</b></td><td>Full 3D projection using the camera view-projection matrix.</td></tr>
  <tr><td><b>Entity Registry</b></td><td>Centralized tracking with distance calculation, visibility flags, and cleanup.</td></tr>
  <tr><td><b>DirectX11 Overlay</b></td><td>Transparent overlay window ready for ESP integration.</td></tr>
  <tr><td><b>Auto Game Detection</b></td><td>Demo automatically detects running Unity games by scanning for <code>UnityPlayer.dll</code>.</td></tr>
</table>

<br>

<h2 id="architecture">Architecture</h2>

<pre>
Nocturne/
├── src/core/           # Process, Memory, Module, Pattern
│   ├── process.h/cpp       # Attach/detach, PID enumeration
│   ├── memory.h/cpp        # ReadProcessMemory wrappers
│   ├── module.h/cpp        # Module base/size enumeration
│   └── pattern.h/cpp       # AOB scanning with wildcards
│
├── src/unity/          # Unity-specific structure readers
│   ├── unity_structs.h       # Offset definitions
│   ├── unity_transform.h/cpp # Transform reader
│   ├── unity_camera.h/cpp    # Camera reader
│   ├── unity_skeleton.h/cpp  # Bone hierarchy reader
│   ├── unity_entity.h        # Entity/Player structs
│   └── unity_resolver.h/cpp  # GOM & Camera resolver
│
├── src/math/           # Math primitives
│   ├── vec3.h                # Vec3, Vec2, Vec4
│   ├── matrix.h              # Matrix4x4
│   └── projection.h/cpp      # World-to-screen
│
├── src/registry/       # High-level entity management
│   ├── entity_registry.h/cpp # Entity/Player lists
│   └── entity_scanner.h/cpp  # Scan orchestration
│
├── src/overlay/        # Rendering
│   └── overlay.h/cpp         # DirectX11 transparent window
│
└── src/demo/           # Console demo
    └── demo.cpp              # Auto-detects Unity games
</pre>

<br>

<h2 id="quick-start">Quick Start</h2>

<h3>1. Build</h3>

<pre><code>cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
</code></pre>

<p>Or run <code>build.bat</code> for automatic Visual Studio detection.</p>

<h3>2. Configure Offsets</h3>

<p>Dump your target game with Cheat Engine or ReClass. Update these files:</p>

<table>
  <tr><th>File</th><th>What to change</th></tr>
  <tr><td><code>src/unity/unity_structs.h</code></td><td>Offsets for Transform, Camera, GameObject</td></tr>
  <tr><td><code>src/unity/unity_resolver.cpp</code></td><td>AOB patterns for GameObjectManager and Camera</td></tr>
</table>

<h3>3. Run the Demo</h3>

<p><b>Automatic detection (no arguments):</b></p>
<pre><code>NocturneDemo.exe</code></pre>
<p>The demo will scan for running Unity games and present a numbered list. Select one to attach.</p>

<pre><code>NocturneDemo.exe TargetGame.exe
&gt; scan      # List all entities
&gt; players   # Filter for players
&gt; camera    # Show camera info
&gt; w2s       # Test world-to-screen
&gt; quit
</code></pre>

<h3>4. Use in Your Project</h3>

<pre><code>#include &lt;nocturne/core/process.h&gt;
#include &lt;nocturne/registry/entity_scanner.h&gt;

nocturne::core::Process proc;
proc.attach(L"TargetGame.exe");

nocturne::core::Memory mem(&amp;proc);
nocturne::registry::EntityScanner scanner(&amp;proc, &amp;mem);
scanner.init();
scanner.scan_players({"Player", "Bot", "NPC"});

for (const auto&amp; p : scanner.registry().players()) {
    auto screen = nocturne::math::Projection::world_to_screen(
        p.position, 1920, 1080, viewMatrix
    );
    if (screen.onScreen) {
        // Draw your ESP here
    }
}
</code></pre>

<br>

<h2 id="build">Build</h2>

<h3>Requirements</h3>

<table>
  <tr><th>Tool</th><th>Version</th><th>Purpose</th></tr>
  <tr><td>Windows</td><td>10/11 x64</td><td>Target platform</td></tr>
  <tr><td>Visual Studio</td><td>2019, 2022, 2026+</td><td>Compiler &amp; Windows SDK</td></tr>
  <tr><td>CMake</td><td>3.16+</td><td>Build generation</td></tr>
</table>

<p><b>Visual Studio Workload:</b> <code>Desktop development with C++</code></p>

<h3>Methods</h3>

<p><b>Method 1 — Automatic (Recommended):</b></p>
<pre><code>build.bat
</code></pre>
<p>Automatically detects Visual Studio 2019 / 2022 / 2026, cleans stale CMake cache, and falls back through compatible generators.</p>

<p><b>Method 2 — CMake Manual:</b></p>
<pre><code># VS 2026
cmake -B build -S . -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release

VS 2022

cmake -B build -S . -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

VS 2019

cmake -B build -S . -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release
</code></pre>

<p><b>Method 3 — MinGW (No VS):</b></p>
<pre><code>cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
</code></pre>

<h3>Output</h3>

<pre>
build/Release/
├── NocturneDemo.exe      # Console demo with auto-detection
├── NocturneTests.exe     # Unit tests
└── nocturne.lib          # Static library
</pre>

<h3>Run Tests</h3>

<pre><code>build/Release/NocturneTests.exe
</code></pre>

<p>Expected output:</p>
<pre>
=== Nocturne Test Runner ===
[PASS] Vec3 addition
[PASS] Vec3 subtraction
[PASS] Vec3 scalar multiply
[PASS] Vec3 dot product
[PASS] Vec3 distance
[PASS] Matrix identity multiply
[PASS] Projection returns valid
[PASS] Registry placeholder
=== RESULTADO: 8 passaram, 0 falharam ===
</pre>

<br>

<h2 id="configuration">Configuration</h2>

<h3>Finding Offsets for Your Target</h3>

<p>Nocturne ships with <b>reference offsets</b> for Unity 2020–2022 x64. Every game compiles differently. You <b>must</b> update these.</p>

<h4>Step 1: Locate GameObjectManager</h4>

<p>Open Cheat Engine, attach to your target, and scan <code>UnityPlayer.dll</code> for the GameObjectManager pattern. Common patterns:</p>

<pre>
Unity 2020-2021: 48 8B 05 ?? ?? ?? ?? 48 8B 48 08 48 8B 01 FF 50 ?? 48 85 C0
Unity 2022+:     48 8B 0D ?? ?? ?? ?? 48 8B 89 ?? ?? ?? ?? 48 85 C9 74 ??
</pre>

<h4>Step 2: Verify Transform Offsets</h4>

<p>Dump a known <code>Transform</code> object and verify:</p>

<table>
  <tr><th>Field</th><th>Common Offset</th><th>How to Verify</th></tr>
  <tr><td>Position</td><td><code>0x90</code></td><td>Search for your player's XYZ in memory, backtrace to Transform base</td></tr>
  <tr><td>Rotation</td><td><code>0x9C</code></td><td>Quaternion follows position (16 bytes)</td></tr>
  <tr><td>Scale</td><td><code>0xAC</code></td><td>Vector3 after rotation</td></tr>
</table>

<h4>Step 3: Verify Camera Offsets</h4>

<table>
  <tr><th>Field</th><th>Common Offset</th><th>How to Verify</th></tr>
  <tr><td>View Matrix</td><td><code>0x2E4</code> or <code>0x300</code></td><td>Look for 4x4 float matrix near Camera base</td></tr>
  <tr><td>FOV</td><td><code>0x40</code></td><td>Float value matching in-game FOV</td></tr>
  <tr><td>Near Clip</td><td><code>0x44</code></td><td>Usually 0.1 or 0.3</td></tr>
  <tr><td>Far Clip</td><td><code>0x48</code></td><td>Usually 1000.0</td></tr>
</table>

<h4>Step 4: Update the Code</h4>

<p>Edit <code>src/unity/unity_structs.h</code>:</p>

<pre><code>struct Transform {
    static constexpr uintptr_t position_offset = 0x90;   // &lt;-- your offset
    static constexpr uintptr_t rotation_offset = 0x9C;   // &lt;-- your offset
    static constexpr uintptr_t scale_offset    = 0xAC;   // &lt;-- your offset
};
</code></pre>

<p>Edit <code>src/unity/unity_resolver.cpp</code>:</p>

<pre><code>auto result = core::Pattern::scan_module(process_, L"UnityPlayer.dll",
    "48 8B 05 ?? ?? ?? ?? 48 8B 48 08 48 8B 01 FF 50 ?? 48 85 C0");  // &lt;-- your pattern
</code></pre>

<br>

<h2 id="troubleshooting">Troubleshooting</h2>

<h3>Build Errors</h3>

<table>
  <tr><th>Error</th><th>Cause</th><th>Fix</th></tr>
  <tr><td><code>'cmake' is not recognized</code></td><td>CMake not in PATH</td><td>Add CMake <code>bin</code> folder to PATH or use VS Developer Command Prompt</td></tr>
  <tr><td><code>Generator not found</code></td><td>Wrong CMake generator</td><td>Use <code>"Visual Studio 17 2022"</code> for VS 2022, <code>"Visual Studio 16 2019"</code> for VS 2019</td></tr>
  <tr><td><code>Platform x64 not supported</code></td><td>Using NMake generator</td><td>Force VS generator: <code>cmake -G "Visual Studio 17 2022"</code></td></tr>
  <tr><td><code>Cannot open include file</code></td><td>Missing Windows SDK</td><td>Install "Windows 10/11 SDK" via Visual Studio Installer</td></tr>
  <tr><td><code>Unresolved external symbol</code></td><td>Missing libs</td><td>Ensure <code>d3d11.lib</code> and <code>dwmapi.lib</code> are linked (already in .vcxproj)</td></tr>
  <tr><td><code>std::vector&lt;bool&gt; has no member data</code></td><td>Using outdated source</td><td>Update to latest main — fixed in recent commits</td></tr>
  <tr><td><code>CMakeCache.txt mismatch</code></td><td>Stale cache from different generator</td><td>Run <code>build.bat</code> (auto-cleans) or delete <code>build/</code> manually</td></tr>
  <tr><td><code>Visual Studio with C++ not found</code></td><td>VS installed in non-standard path or missing workload</td><td>Install "Desktop development with C++" workload via VS Installer</td></tr>
</table>

<h3>Runtime Errors</h3>

<table>
  <tr><th>Error</th><th>Cause</th><th>Fix</th></tr>
  <tr><td><code>Failed to attach to process</code></td><td>Game not running / wrong name</td><td>Check exact process name in Task Manager</td></tr>
  <tr><td><code>Scanner init failed</code></td><td>Patterns not found</td><td>Update AOB patterns in <code>unity_resolver.cpp</code> for your Unity version</td></tr>
  <tr><td><code>Camera not resolved</code></td><td>Wrong Camera pattern</td><td>Dump Camera.main pointer chain manually</td></tr>
  <tr><td><code>All positions are zero</code></td><td>Wrong Transform offsets</td><td>Re-dump Transform structure in Cheat Engine</td></tr>
  <tr><td><code>W2S returns off-screen</code></td><td>Wrong view matrix offset</td><td>Verify Camera view matrix offset; try <code>0x2E4</code>, <code>0x300</code>, or <code>0x7C</code></td></tr>
  <tr><td><code>Overlay not showing</code></td><td>DWM composition disabled</td><td>Ensure Windows Aero/DWM is enabled</td></tr>
  <tr><td><code>No Unity games found</code></td><td>No Unity process running</td><td>Open a Unity game before running <code>NocturneDemo.exe</code> without arguments</td></tr>
</table>

<h3>Git Push Errors</h3>

<table>
  <tr><th>Error</th><th>Fix</th></tr>
  <tr><td><code>Repository not found</code></td><td>Verify username (c1pheraxx, not cipheraxx) and that repo exists on GitHub</td></tr>
  <tr><td><code>Authentication failed</code></td><td>Use Personal Access Token instead of password: GitHub Settings → Developer settings → Tokens</td></tr>
  <tr><td><code>LF will be replaced by CRLF</code></td><td>Normal warning on Windows. Run: <code>git config --global core.autocrlf true</code></td></tr>
</table>

<br>

<h2 id="faq">FAQ</h2>

<table>
  <tr>
    <td><b>Does this work for IL2CPP games?</b></td>
    <td>v0.1 targets Mono/standard Unity. IL2CPP support is planned for v0.3. Structure layouts differ in IL2CPP builds.</td>
  </tr>
  <tr>
    <td><b>Can I get banned using this?</b></td>
    <td>This is an external read-only tool. It does not write memory, inject DLLs, or hook functions. However, some games have kernel-level anti-cheats that detect external memory reading. Use at your own risk.</td>
  </tr>
  <tr>
    <td><b>Why are the offsets wrong for my game?</b></td>
    <td>Unity versions, build settings, and obfuscation change offsets. The included values are references. You must dump your specific target.</td>
  </tr>
  <tr>
    <td><b>Can I use this for non-Unity games?</b></td>
    <td>The core layer (Process, Memory, Pattern) works for any Windows process. The Unity layer is Unity-specific.</td>
  </tr>
  <tr>
    <td><b>Where do I find the GameObjectManager?</b></td>
    <td>In Cheat Engine, open <code>UnityPlayer.dll</code>, search for byte arrays matching the patterns above. The first valid result that points to a structure with active GameObjects is usually correct.</td>
  </tr>
  <tr>
    <td><b>The demo crashes without arguments</b></td>
    <td>Update to the latest version. The demo now auto-detects Unity games when run without arguments.</td>
  </tr>
</table>

<br>

<h2 id="roadmap">Roadmap</h2>

<h3>v0.1 <img src="https://img.shields.io/badge/status-released-22c55e?style=flat-square"/></h3>

<ul>
  <li>✓ Process/memory core</li>
  <li>✓ Pattern scanning (AOB)</li>
  <li>✓ Unity Transform reader</li>
  <li>✓ Unity Camera reader</li>
  <li>✓ Skeleton enumeration</li>
  <li>✓ Entity/Player registry</li>
  <li>✓ World-to-screen projection</li>
  <li>✓ DirectX11 overlay skeleton</li>
  <li>✓ Console demo</li>
  <li>✓ Console demo with auto-detection</li>
  <li>✓ MSVC compilation fixes (<code>vector&lt;bool&gt;</code>, missing includes)</li>
  <li>✓ Visual Studio 2026 support</li>
</ul>

<h3>v0.2 <img src="https://img.shields.io/badge/status-in%20progress-f59e0b?style=flat-square"/></h3>

<ul>
  <li>○ ImGui integration for overlay</li>
  <li>○ JSON configuration for offsets</li>
  <li>○ ESP box and skeleton drawing</li>
  <li>○ Distance-based filtering</li>
</ul>

<h3>v0.3 <img src="https://img.shields.io/badge/status-planned-8b5cf6?style=flat-square"/></h3>

<ul>
  <li>○ IL2CPP metadata parsing</li>
  <li>○ Mono runtime enumeration</li>
  <li>○ Automated offset dumper</li>
  <li>○ Plugin system for game-specific modules</li>
</ul>

<br>

<h2 id="contributing">Contributing</h2>

<p>See <a href="CONTRIBUTING.md">CONTRIBUTING.md</a>.</p>

<p>Open an issue before major changes. Include:</p>
<ul>
  <li>Windows version</li>
  <li>Target game and Unity version</li>
  <li>Offsets and patterns used</li>
  <li>Steps to reproduce</li>
</ul>

<br>

<h2 id="contributors">Contributors</h2>

<p>See <a href="CONTRIBUTORS.md">CONTRIBUTORS.md</a> for the full list of people who have contributed to this project.</p>

<br>

<h2 id="license">License</h2>

<p>MIT — see <a href="LICENSE">LICENSE</a>.</p>

<br>

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0d1117&height=100&section=footer"/>
</p>
