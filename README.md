# Windows Chaos Crasher
Overview

Windows Chaos Crasher is a C program designed exclusively for cybersecurity demonstrations in controlled environments. This tool methodically terminates accessible Windows system processes with administrative privileges, aiming to trigger a Blue Screen of Death (BSOD) 

by disrupting critical system components such as csrss.exe, smss.exe, or winlogon.exe. It is built to showcase the severe consequences of improper process manipulation in a Windows environment, making it an ideal tool for educational purposes, security training, or 

penetration testing scenarios.

⚠️ CRITICAL WARNING ⚠️This program is EXTREMELY DANGEROUS and can cause system instability, data loss, or permanent damage if misused. It must ONLY be executed in an isolated virtual machine (e.g., VirtualBox, VMware) with snapshots enabled for easy recovery. Use in 

production environments or without explicit authorization is strictly prohibited. The author is not responsible for any damage caused by improper use.

Features

Administrative Privilege Check: Ensures the program runs with elevated permissions, exiting gracefully if not.

SeDebugPrivilege Activation: Enables access to protected system processes using Windows API calls.

Robust Error Handling: Skips inaccessible processes (e.g., due to system protections) and continues targeting others to maximize disruption.

Methodical Chaos: Iteratively terminates processes with a slight delay (50ms) to prolong the demonstration effect, ensuring a critical process is eventually terminated to trigger a BSOD.

Persistent Execution: Runs in an infinite loop to guarantee system instability, even if initial attempts fail.

How It Works

Privilege Escalation: The program verifies administrative privileges and activates SeDebugPrivilege to gain access to system-level processes.

Process Enumeration: Utilizes CreateToolhelp32Snapshot to list all running processes.

Targeted Termination: Attempts to terminate each process using TerminateProcess, skipping the current process to avoid self-destruction.

Error Resilience: Handles access-denied errors by moving to the next process, ensuring continuous operation until a critical process is terminated.

BSOD Trigger: Once a critical process (e.g., csrss.exe) is terminated, Windows detects the failure and triggers a BSOD with an error like CRITICAL_PROCESS_DIED.

Prerequisites

Operating System: Windows (tested on Windows 10/11).

Compiler: MinGW or MSYS2 with gcc.

Administrative Privileges: Must run as administrator to access system processes.

Virtual Machine: Strongly recommended to avoid damage to physical systems.

Compilation

Save the code as mayhem.c.

Open a command prompt or PowerShell in the directory containing mayhem.c.

Compile using gcc:gcc -o mayhem.exe mayhem.c -lpsapi -DUNICODE



-lpsapi: Links the Process Status API library for process enumeration.

-DUNICODE: Ensures Unicode compatibility for Windows API calls.



Usage


Run as Administrator:

Right-click on Command Prompt or PowerShell and select "Run as administrator".

Navigate to the directory containing mayhem.exe:cd path/to/your/directory


Execute the program:mayhem.exe




Expected Outcome:

The program will begin terminating accessible processes.

Within 1–5 seconds, a critical process will likely be terminated, triggering a BSOD.

The system will crash with an error such as CRITICAL_PROCESS_DIED.



Safety Guidelines

Use a Virtual Machine: Always run this program in a virtualized environment with snapshots to restore the system post-crash.

Backup Data: Ensure no critical data is present on the test system.

Authorization: Only use this tool in environments where you have explicit permission to perform such tests.

Educational Purpose: This code is intended for learning and demonstration purposes in controlled settings, such as cybersecurity training or academic exercises.

Troubleshooting

Compilation Errors: Ensure gcc is in your PATH and -lpsapi is included. Verify MinGW/MSYS2 installation.

No BSOD: Confirm the program is running with administrative privileges (right-click > "Run as administrator").

Access Denied Errors: Some processes are heavily protected by Windows; the program automatically skips these and continues.

Contributing

This project is for educational purposes and not actively maintained. If you have suggestions or improvements, feel free to fork the repository and submit a pull request. Please ensure all contributions adhere to ethical and legal standards.

License

This project is provided as-is, with no warranty. Use at your own risk. The author is not liable for any damage caused by this software. See the LICENSE file for details (if applicable).

Acknowledgments

Built for cybersecurity enthusiasts and educators.

Inspired by the need to demonstrate the fragility of critical system processes in Windows.

Stay safe, and happy crashing (in a VM)!
