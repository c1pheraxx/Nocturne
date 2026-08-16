#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <nocturne/core/process.h>
#include <nocturne/registry/entity_scanner.h>
#include <nocturne/math/projection.h>

namespace nocturne::demo {

struct UnityGame {
    std::wstring name;
    DWORD pid;
};

std::vector<UnityGame> find_unity_games() {
    std::vector<UnityGame> games;
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return games;
    
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    
    if (Process32FirstW(snapshot, &pe)) {
        do {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
            if (!hProcess) continue;
            
            HMODULE hMods[1024];
            DWORD cbNeeded;
            bool isUnity = false;
            
            if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
                for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
                    wchar_t modName[MAX_PATH];
                    if (GetModuleBaseNameW(hProcess, hMods[i], modName, sizeof(modName) / sizeof(wchar_t))) {
                        if (_wcsicmp(modName, L"UnityPlayer.dll") == 0) {
                            isUnity = true;
                            break;
                        }
                    }
                }
            }
            
            CloseHandle(hProcess);
            
            if (isUnity) {
                games.push_back({pe.szExeFile, pe.th32ProcessID});
            }
        } while (Process32NextW(snapshot, &pe));
    }
    
    CloseHandle(snapshot);
    return games;
}

} // namespace nocturne::demo

int wmain(int argc, wchar_t* argv[]) {
    using namespace nocturne;
    
    std::wstring target_name;
    
    if (argc < 2) {
        std::wcout << L"========================================" << std::endl;
        std::wcout << L" Nocturne - Detector de Jogos Unity" << std::endl;
        std::wcout << L"========================================" << std::endl << std::endl;
        
        auto games = demo::find_unity_games();
        
        if (games.empty()) {
            std::wcout << L"[AVISO] Nenhum jogo Unity encontrado em execucao." << std::endl;
            std::wcout << L"[INFO] Abra um jogo Unity e rode novamente." << std::endl << std::endl;
            std::wcout << L"Uso: NocturneDemo.exe <NomeDoJogo.exe>" << std::endl;
            std::wcout << L"   ou: NocturneDemo.exe <PID>" << std::endl;
            system("pause");
            return 1;
        }
        
        std::wcout << L"Jogos Unity detectados:" << std::endl;
        std::wcout << L"----------------------------------------" << std::endl;
        for (size_t i = 0; i < games.size(); ++i) {
            std::wcout << L" [" << (i + 1) << L"] " << games[i].name 
                       << L"  (PID: " << games[i].pid << L")" << std::endl;
        }
        std::wcout << L"----------------------------------------" << std::endl;
        std::wcout << L"Digite o numero do jogo: ";
        
        int choice;
        std::wcin >> choice;
        
        if (choice < 1 || choice > static_cast<int>(games.size())) {
            std::wcout << L"[ERRO] Escolha invalida." << std::endl;
            system("pause");
            return 1;
        }
        
        target_name = games[choice - 1].name;
        std::wcout << L"[OK] Selecionado: " << target_name << std::endl << std::endl;
    } else {
        target_name = argv[1];
    }
    
    core::Process proc;
    if (!proc.attach(target_name.c_str())) {
        std::wcout << L"[ERRO] Falha ao anexar ao processo: " << target_name << std::endl;
        system("pause");
        return 1;
    }
    
    std::wcout << L"[OK] Anexado a: " << target_name << std::endl;
    
    core::Memory mem(&proc);
    registry::EntityScanner scanner(&proc, &mem);
    
    if (!scanner.init()) {
        std::wcout << L"[ERRO] Falha ao inicializar scanner." << std::endl;
        std::wcout << L"[INFO] Verifique se os offsets/patterns estao corretos." << std::endl;
        system("pause");
        return 1;
    }
    
    std::wcout << L"[OK] Scanner inicializado." << std::endl << std::endl;
    std::wcout << L"Comandos disponiveis:" << std::endl;
    std::wcout << L"  scan      - Listar todas as entidades" << std::endl;
    std::wcout << L"  players   - Filtrar jogadores" << std::endl;
    std::wcout << L"  camera    - Mostrar info da camera" << std::endl;
    std::wcout << L"  w2s       - Testar world-to-screen" << std::endl;
    std::wcout << L"  quit      - Sair" << std::endl;
    std::wcout << std::endl;
    
    std::string cmd;
    while (true) {
        std::wcout << L"> ";
        std::cin >> cmd;
        
        if (cmd == "quit" || cmd == "exit") break;
        
        if (cmd == "scan") {
            scanner.scan_all();
            std::wcout << L"Entidades encontradas: " << scanner.registry().entities().size() << std::endl;
        } else if (cmd == "players") {
            scanner.scan_players({L"Player", L"Bot", L"NPC"});
            std::wcout << L"Jogadores encontrados: " << scanner.registry().players().size() << std::endl;
        } else if (cmd == "camera") {
            auto cam = scanner.camera_info();
            std::wcout << L"Camera resolvida: " << (cam.valid ? L"SIM" : L"NAO") << std::endl;
        } else if (cmd == "w2s") {
            auto cam = scanner.camera_info();
            if (!cam.valid) {
                std::wcout << L"[ERRO] Camera nao resolvida." << std::endl;
                continue;
            }
            math::Vec3 world(0, 0, 0);
            auto screen = math::Projection::world_to_screen(world, 1920, 1080, cam.view_matrix);
            std::wcout << L"W2S (0,0,0) -> (" << screen.x << L", " << screen.y << L") onScreen=" << screen.on_screen << std::endl;
        } else {
            std::wcout << L"Comando desconhecido: " << cmd.c_str() << std::endl;
        }
    }
    
    proc.detach();
    return 0;
}
