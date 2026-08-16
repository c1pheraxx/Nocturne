# API Reference

## Process

```cpp
nocturne::core::Process proc;
proc.attach(L"Game.exe");        // por nome
proc.attach(1234);              // por PID
proc.is_attached();              // bool
proc.pid();                       // uint32_t
proc.handle();                    // HANDLE
```

## Memory

```cpp
nocturne::core::Memory mem(&proc);
mem.read<int>(0x1234);                              // read tipado
mem.read_string(addr);                               // string
mem.read_chain(base, {0x10, 0x20, 0x8});           // pointer chain
```

## Pattern Scan

```cpp
auto result = nocturne::core::Pattern::scan(&proc, start, size, "48 8B 05 ?? ?? ?? ??");
if (result.found) uintptr_t addr = result.address;
```

## UnityResolver

```cpp
nocturne::unity::UnityResolver resolver(&proc, &mem);
resolver.init();                                    // resolve GOM + Camera

uintptr_t camera = resolver.resolve_camera();
auto entities = resolver.resolve_entities();
auto players = resolver.resolve_players({"Player"});
```

## TransformReader

```cpp
nocturne::unity::TransformReader tr(&mem);
auto pos = tr.position(transformAddr);
auto rot = tr.rotation_euler(transformAddr);
auto scl = tr.scale(transformAddr);
```

## CameraReader

```cpp
nocturne::unity::CameraReader cr(&mem);
auto vm = cr.view_matrix(cameraAddr);
auto fov = cr.fov(cameraAddr);
auto pos = cr.position(cameraAddr);
```

## SkeletonReader

```cpp
nocturne::unity::SkeletonReader sr(&mem);
auto bones = sr.read_skeleton(rootTransformAddr);
auto head = bones["Head"];
```

## Projection

```cpp
auto screen = nocturne::math::Projection::world_to_screen(
    worldPos, 1920, 1080, viewMatrix
);
// screen.x, screen.y, screen.depth, screen.onScreen, screen.valid
```

## EntityScanner

```cpp
nocturne::registry::EntityScanner scanner(&proc, &mem);
scanner.init();
scanner.scan_players({"Player", "Bot"});

for (auto& p : scanner.registry().players()) {
    std::cout << p.name << " " << p.position.x << std::endl;
}
```
