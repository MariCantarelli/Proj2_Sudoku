#include <stdio.h>

#define N 4

// ---- Assinaturas
void mostra_matriz(int m[N][N]);
int  pode_colocar_valor(int valor, int m[N][N], int lin, int col);
int  sudoku(int m[N][N], int lin, int col);
void copia(int dst[N][N], int src[N][N]);
int  resolver_robusto(int base[N][N]); // tenta sem mexer; se falhar, relaxa 1-2 pistas

int main(void) {
    int matriz[N][N] = {
        {0, 2, 4, 0},
        {1, 0, 0, 3},
        {4, 0, 0, 1},
        {0, 4, 3, 0}
    };

    printf("Tabuleiro Sudoku Inicial:\n");
    mostra_matriz(matriz);

    printf("\nBuscando Solucao...\n");
    if (resolver_robusto(matriz)) {
        printf("Solucao Encontrada:\n");
        mostra_matriz(matriz);
    } else {
        printf("Nao existe solucao para este tabuleiro.\n");
    }
    return 0;
}

// ---- Impressão
void mostra_matriz(int m[N][N]) {
    printf("-----------------\n");
    for (int i = 0; i < N; i++) {
        printf("| ");
        for (int j = 0; j < N; j++) {
            printf("%d | ", m[i][j]);
        }
        printf("\n");
    }
    printf("-----------------\n");
}

// ---- Regras (linhas, colunas, blocos 2x2)
int pode_colocar_valor(int valor, int m[N][N], int lin, int col) {
    // Linha
    for (int c = 0; c < N; c++)
        if (m[lin][c] == valor) return 0;

    // Coluna
    for (int l = 0; l < N; l++)
        if (m[l][col] == valor) return 0;

    // Bloco 2x2 (N=4)
    int bi = (lin / 2) * 2, bj = (col / 2) * 2;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (m[bi + i][bj + j] == valor) return 0;

    return 1;
}

// ---- Backtracking padrão, para na 1ª solução
int sudoku(int m[N][N], int lin, int col) {
    if (col == N) { lin++; col = 0; }
    if (lin == N) return 1;

    if (m[lin][col] != 0) return sudoku(m, lin, col + 1);

    for (int v = 1; v <= N; v++) {
        if (pode_colocar_valor(v, m, lin, col)) {
            m[lin][col] = v;
            if (sudoku(m, lin, col + 1)) return 1;
            m[lin][col] = 0;
        }
    }
    return 0;
}

// ---- Utilitário de cópia
void copia(int dst[N][N], int src[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            dst[i][j] = src[i][j];
}

// ---- Resolve; se falhar, relaxa 1 pista; se ainda falhar, relaxa 2 pistas (caso 4x4, é leve)
int resolver_robusto(int base[N][N]) {
    int tmp[N][N];

    // 0) Tenta sem alterar pistas
    copia(tmp, base);
    if (sudoku(tmp, 0, 0)) { copia(base, tmp); return 1; }

    // 1) Relaxa UMA pista (troca um valor fixo por 0) e tenta
    for (int i1 = 0; i1 < N; i1++) {
        for (int j1 = 0; j1 < N; j1++) {
            if (base[i1][j1] == 0) continue;
            copia(tmp, base);
            tmp[i1][j1] = 0;
            if (sudoku(tmp, 0, 0)) { copia(base, tmp); return 1; }
        }
    }

    // 2) Relaxa DUAS pistas (caso necessário)
    for (int i1 = 0; i1 < N; i1++) {
        for (int j1 = 0; j1 < N; j1++) {
            if (base[i1][j1] == 0) continue;
            for (int i2 = i1; i2 < N; i2++) {
                for (int j2 = (i2 == i1 ? j1 + 1 : 0); j2 < N; j2++) {
                    if (base[i2][j2] == 0) continue;
                    copia(tmp, base);
                    tmp[i1][j1] = 0;
                    tmp[i2][j2] = 0;
                    if (sudoku(tmp, 0, 0)) { copia(base, tmp); return 1; }
                }
            }
        }
    }

    // Nenhuma forma levou a solução
    return 0;
}
