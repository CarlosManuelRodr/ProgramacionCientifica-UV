squared_norm_1(Vector3):             # @squared_norm_1(Vector3)
        push    rbp
        mov     rbp, rsp
        mov     qword ptr [rbp - 32], rdi
        mov     dword ptr [rbp - 24], esi
        mov     rax, qword ptr [rbp - 32]
        mov     qword ptr [rbp - 16], rax
        mov     ecx, dword ptr [rbp - 24]
        mov     dword ptr [rbp - 8], ecx
        mov     ecx, dword ptr [rbp - 16]
        imul    ecx, dword ptr [rbp - 16]
        mov     edx, dword ptr [rbp - 12]
        imul    edx, dword ptr [rbp - 12]
        add     ecx, edx
        mov     edx, dword ptr [rbp - 8]
        imul    edx, dword ptr [rbp - 8]
        add     ecx, edx
        mov     eax, ecx
        pop     rbp
        ret
squared_norm_2(Vector3 const&):           # @squared_norm_2(Vector3 const&)
        push    rbp
        mov     rbp, rsp
        mov     qword ptr [rbp - 8], rdi
        mov     rax, qword ptr [rbp - 8]
        mov     ecx, dword ptr [rax]
        mov     rax, qword ptr [rbp - 8]
        imul    ecx, dword ptr [rax]
        mov     rax, qword ptr [rbp - 8]
        mov     edx, dword ptr [rax + 4]
        mov     rax, qword ptr [rbp - 8]
        imul    edx, dword ptr [rax + 4]
        add     ecx, edx
        mov     rax, qword ptr [rbp - 8]
        mov     edx, dword ptr [rax + 8]
        mov     rax, qword ptr [rbp - 8]
        imul    edx, dword ptr [rax + 8]
        add     ecx, edx
        mov     eax, ecx
        pop     rbp
        ret
squared_norm_3(Vector3 const*):           # @squared_norm_3(Vector3 const*)
        push    rbp
        mov     rbp, rsp
        mov     qword ptr [rbp - 8], rdi
        mov     rax, qword ptr [rbp - 8]
        mov     ecx, dword ptr [rax]
        mov     rax, qword ptr [rbp - 8]
        imul    ecx, dword ptr [rax]
        mov     rax, qword ptr [rbp - 8]
        mov     edx, dword ptr [rax + 4]
        mov     rax, qword ptr [rbp - 8]
        imul    edx, dword ptr [rax + 4]
        add     ecx, edx
        mov     rax, qword ptr [rbp - 8]
        mov     edx, dword ptr [rax + 8]
        mov     rax, qword ptr [rbp - 8]
        imul    edx, dword ptr [rax + 8]
        add     ecx, edx
        mov     eax, ecx
        pop     rbp
        ret
main:                                   # @main
        push    rbp
        mov     rbp, rsp
        sub     rsp, 48
        mov     dword ptr [rbp - 4], 0
        mov     dword ptr [rbp - 16], 5
        mov     dword ptr [rbp - 12], 10
        mov     dword ptr [rbp - 8], 9
        mov     rax, qword ptr [rbp - 16]
        mov     qword ptr [rbp - 32], rax
        mov     ecx, dword ptr [rbp - 8]
        mov     dword ptr [rbp - 24], ecx
        mov     rax, qword ptr [rbp - 32]
        mov     qword ptr [rbp - 48], rax
        mov     ecx, dword ptr [rbp - 24]
        mov     dword ptr [rbp - 40], ecx
        mov     rdi, qword ptr [rbp - 48]
        mov     esi, dword ptr [rbp - 40]
        call    squared_norm_1(Vector3)
        add     rsp, 48
        pop     rbp
        ret