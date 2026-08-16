#include <iostream>
#include <cstring>
#include <windows.h>
#include "../src/core/process.h"
#include "../src/core/memory.h"
#include "../src/registry/entity_scanner.h"
#include "../src/unity/unity_camera.h"
#include "../src/unity/unity_transform.h"
#include "../src/math/projection.h"

using namespace nocturne;

void print_usage() {
    std::cout << "=== Nocturne Demo ===" << std::endl;
    std::cout << "Uso: NocturneDemo.exe <nome_do_processo.exe>" << std::endl;
    std::cout << "Exemplo: NocturneDemo.exe MyGame.exe" << std::endl;
    std::cout << std::endl;
    std::cout << "Comandos:" << std::endl;
    std::cout << "  scan     - Escaneia entidades no jogo" << std::endl;
    std::cout << "  players  - Escaneia jogadores" << std::endl;
    std::cout << "  camera   - Mostra info da camera" << std::endl;
    std::cout << "  w2s      - Testa world-to-screen na entidade mais proxima" << std::endl;
    std::cout << "  quit     - Sai" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::wstring processName(argv[1], argv[1] + strlen(argv[1]));

    core::Process process;
    if (!process.attach(processName)) {
        std::cerr << "Falha ao anexar ao processo: " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Anexado a " << argv[1] << " (PID: " << process.pid() << ")" << std::endl;

    core::Memory memory(&process);
    registry::EntityScanner scanner(&process, &memory);

    if (!scanner.init()) {
        std::cerr << "Falha ao inicializar scanner. Verifique se o jogo esta rodando." << std::endl;
        return 1;
    }

    std::cout << "Scanner inicializado. Digite um comando:" << std::endl;

    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;

        if (cmd == "quit" || cmd == "exit") break;

        if (cmd == "scan") {
            scanner.scan();
            auto& reg = scanner.registry();
            std::cout << "Entidades encontradas: " << reg.entity_count() << std::endl;
            for (const auto& e : reg.entities()) {
                std::cout << "  [" << e.id << "] " << e.name
                          << " | Pos(" << e.position.x << ", " << e.position.y << ", " << e.position.z << ")"
                          << " | Dist: " << e.distance << std::endl;
            }
        }
        else if (cmd == "players") {
            scanner.scan_players({"Player", "Character", "Bot", "NPC", "Hero"});
            auto& reg = scanner.registry();
            std::cout << "Jogadores encontrados: " << reg.player_count() << std::endl;
            for (const auto& p : reg.players()) {
                std::cout << "  [" << p.id << "] " << p.name
                          << " | Pos(" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")"
                          << " | Local: " << (p.is_local ? "Sim" : "Nao")
                          << " | NPC: " << (p.is_npc ? "Sim" : "Nao")
                          << " | Bones: " << p.skeleton.size() << std::endl;
            }
        }
        else if (cmd == "camera") {
            unity::UnityResolver resolver(&process, &memory);
            resolver.init();
            unity::CameraReader camReader(&memory);
            uintptr_t camAddr = resolver.resolve_camera();
            if (camAddr) {
                auto pos = camReader.position(camAddr);
                auto fov = camReader.fov(camAddr);
                std::cout << "Camera: " << pos.x << ", " << pos.y << ", " << pos.z
                          << " | FOV: " << fov << std::endl;
            } else {
                std::cout << "Camera nao resolvida." << std::endl;
            }
        }
        else if (cmd == "w2s") {
            scanner.scan_players({"Player", "Character"});
            auto& reg = scanner.registry();
            if (reg.players().empty()) {
                std::cout << "Nenhum jogador encontrado." << std::endl;
                continue;
            }

            unity::UnityResolver resolver(&process, &memory);
            resolver.init();
            unity::CameraReader camReader(&memory);
            uintptr_t camAddr = resolver.resolve_camera();
            if (!camAddr) {
                std::cout << "Camera nao resolvida." << std::endl;
                continue;
            }

            auto vm = camReader.view_matrix(camAddr);
            auto& p = reg.players()[0];
            auto screen = math::Projection::world_to_screen(p.position, 1920, 1080, vm);

            std::cout << "W2S para " << p.name << ":"
                      << " X=" << screen.x << " Y=" << screen.y
                      << " Depth=" << screen.depth
                      << " OnScreen=" << (screen.onScreen ? "Sim" : "Nao") << std::endl;
        }
        else {
            std::cout << "Comando desconhecido: " << cmd << std::endl;
        }
    }

    std::cout << "Saindo..." << std::endl;
    return 0;
}
