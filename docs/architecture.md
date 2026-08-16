# Architecture

## Visão geral

```
┌────────────────────────────────────────┐
│  Demo / Overlay / User Tool            │
├────────────────────────────────────────┤
│  Registry (EntityScanner)              │
├────────────────────────────────────────┤
│  Unity Readers (Resolver, Transform,     │
│  Camera, Skeleton)                     │
├────────────────────────────────────────┤
│  Core (Process, Memory, Module,         │
│  Pattern)                              │
├────────────────────────────────────────┤
│  Windows API                           │
└────────────────────────────────────────┘
```

## Core

- **Process** — `OpenProcess`, enumeração via Toolhelp32.
- **Memory** — `ReadProcessMemory` com templates, chain reads, string reads.
- **Module** — Enumera módulos do processo alvo.
- **Pattern** — AOB scan com wildcards (`??`).

## Unity

- **UnityResolver** — Encontra `GameObjectManager` e `Camera` via pattern scan.
- **TransformReader** — Lê position, rotation, scale de `UnityEngine.Transform`.
- **CameraReader** — Lê viewMatrix, FOV, clip planes de `UnityEngine.Camera`.
- **SkeletonReader** — Percorre hierarquia de `Transform` children para listar bones.

## Math

- **Vec3 / Vec4 / Vec2** — Álgebra básica.
- **Matrix4x4** — Multiplicação ponto-vetor.
- **Projection** — `world_to_screen` via view-projection matrix.

## Registry

- **EntityRegistry** — Mantém listas de `Entity` e `Player`.
- **EntityScanner** — Orquestra `UnityResolver` + readers para popular o registry.

## Overlay

- **Overlay** — Janela transparente WS_EX_LAYERED + DirectX11 swap chain.
- Integrar ImGui para UI real.
