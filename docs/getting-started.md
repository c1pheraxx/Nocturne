# Getting Started

## 1. Build

Siga [BUILD.md](../BUILD.md).

## 2. Encontre seu alvo

Abra o jogo Unity. Anote o nome do processo (ex: `RustClient.exe`, `EscapeFromTarkov.exe`).

## 3. Dump dos offsets

Use Cheat Engine ou ReClass para encontrar:
- `GameObjectManager` dentro de `UnityPlayer.dll`
- `Camera.main` ou `Camera.current`
- Offsets de `Transform` (position, rotation, scale)
- Offset do nome em `GameObject`

## 4. Atualize o Nocturne

Edite `src/unity/unity_structs.h` com os offsets corretos.

Edite `src/unity/unity_resolver.cpp` com os patterns corretos.

## 5. Rode a demo

```cmd
NocturneDemo.exe SeuJogo.exe
> players
> camera
> w2s
```

## 6. Construa sua ferramenta

Link contra `nocturne.lib` e use os headers em `src/`.

```cpp
#include <nocturne/core/process.h>
#include <nocturne/registry/entity_scanner.h>
```
