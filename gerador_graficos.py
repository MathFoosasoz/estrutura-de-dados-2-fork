import os
import subprocess
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


PASTAS_TESTCASES = [
    "casos_almost_reverse_sorted",
    "casos_almost_sorted",
    "casos_anticount",
    "casos_antimerge",
    "casos_antiquick",
    "casos_fully_reversed",
    "casos_random_full_range",
    "casos_random_small_range",
    "casos_repetition_50pct",
]
MAIN_SORTS = "./main"


def contar_elementos(caminho_arquivo):
    with open(caminho_arquivo, "r") as f:
        conteudo = f.read().split()
        return len(conteudo)
 
 
def achar_testcases(pasta):
    test_cases = {}

    if not os.path.exists(pasta):
        print(f"Pasta {pasta} não encontrada")
        return test_cases

    for arquivo in os.listdir(pasta):
        caminho = os.path.join(pasta, arquivo)
        if os.path.isfile(caminho):
            tamanho = contar_elementos(caminho)
            test_cases[tamanho] = caminho

    return test_cases


def executar_benchmarks(test_cases):
    coleta_tempo = []
    coleta_operacoes = []
    coleta_memoria = []
    coleta_score = []
    mapa_heuristica = {}

    for tamanho in sorted(test_cases.keys()):
        arquivo = test_cases[tamanho]

        subprocess.run(
            [MAIN_SORTS, "-i", arquivo, "-b", "--output", "-k"], check=True
        )

        dados_rodada_atual = pd.read_csv("output.csv", index_col="Metric")
        
        with open("escolha_heuristica.txt", "r") as f:
            escolha = f.read().strip()

        escolha_sanitize = escolha.lower().replace("sort", "").strip()
        coluna_escolhida = None

        for col in dados_rodada_atual.columns:
            if escolha_sanitize in col.lower():
                coluna_escolhida = col
                break

        mapa_heuristica[tamanho] = (
            coluna_escolhida if coluna_escolhida else escolha
        )

        d_tempo = dados_rodada_atual.loc["Time Taken"].to_dict()
        d_tempo["Tamanho_N"] = tamanho
        coleta_tempo.append(d_tempo)

        d_ops = (
            dados_rodada_atual.loc["Comparisons"]
            + dados_rodada_atual.loc["Assignments"]
        ).to_dict()
        d_ops["Tamanho_N"] = tamanho
        coleta_operacoes.append(d_ops)

        d_mem = dados_rodada_atual.loc["Memory Allocated"].to_dict()
        d_mem["Tamanho_N"] = tamanho
        coleta_memoria.append(d_mem)

        d_score = dados_rodada_atual.loc["Score"].to_dict()
        d_score["Tamanho_N"] = tamanho
        coleta_score.append(d_score)

    dados_tempo = pd.DataFrame(coleta_tempo).set_index("Tamanho_N").apply(pd.to_numeric, errors='coerce').fillna(0)
    dados_operacoes = pd.DataFrame(coleta_operacoes).set_index("Tamanho_N").apply(pd.to_numeric, errors='coerce').fillna(0)
    dados_memoria = pd.DataFrame(coleta_memoria).set_index("Tamanho_N").apply(pd.to_numeric, errors='coerce').fillna(0)
    dados_score = pd.DataFrame(coleta_score).set_index("Tamanho_N").apply(pd.to_numeric, errors='coerce').fillna(0)

    return dados_tempo, dados_operacoes, dados_memoria, dados_score, mapa_heuristica


def gerar_graficos(dados_tempo, dados_operacoes, dados_memoria, dados_score, mapa_heuristica, nome_pasta):
    config_graficos = [
        (dados_tempo, "Análise de Tempo", "Tempo de Execução (segundos)", True, True),
        (dados_operacoes, "Análise de Operações escala monolog", "Quantidade de Operações (Log)", True, True),
        (dados_memoria, "Análise de Memória Alocada", "Memory Allocated (Bytes)", True, True),
        (dados_score, "Pontuação da Heurística", "Score (Log)", True, False),
    ]

    fig, axes = plt.subplots(2, 2, figsize=(16, 12))

    for idx, (dados, titulo, eixo_y, usar_log, desenhar_estrela) in enumerate(config_graficos):
        ax = axes.flat[idx]
        x_plot = dados.index.values
        
        x_estrelas_heuristica = []
        y_estrelas_heuristica = []

        for algoritmo in dados.columns:
            y_plot = dados[algoritmo].values
            
            if np.all(y_plot == 0):
                continue
            
            if usar_log:
                y_plot = np.clip(y_plot, 1e-8, None)
            
            x_plot = np.clip(x_plot, 1, None)
                
            if len(y_plot) > 0:
                ax.plot(x_plot, y_plot, marker="o", label=algoritmo, linewidth=2, markersize=5)
                
                if desenhar_estrela:
                    for xi, yi in zip(x_plot, y_plot):
                        if mapa_heuristica.get(xi) == algoritmo:
                            x_estrelas_heuristica.append(xi)
                            y_estrelas_heuristica.append(yi)

        if desenhar_estrela and x_estrelas_heuristica:
            ax.scatter(
                x_estrelas_heuristica, 
                y_estrelas_heuristica, 
                color="gold", 
                edgecolor="black", 
                s=150, 
                marker="*", 
                zorder=5, 
                label="Escolha da Heurística"
            )

        if usar_log:
            ax.set_yscale("log")
            
        ax.set_xscale("log")

        ax.set_title(titulo, fontsize=13, fontweight="bold")
        ax.set_xlabel("Tamanho N do Vetor", fontsize=11)
        ax.set_ylabel(eixo_y, fontsize=11)
        ax.grid(True, which="both", linestyle="--", alpha=0.5)
        ax.legend(loc="upper left", fontsize=9)

    plt.tight_layout(h_pad=4.0, w_pad=4.0)
    
    nome_saida = f"analise_{nome_pasta}.png"
    plt.savefig(nome_saida, dpi=300)
    print(f"Gráfico salvo: {nome_saida}")
    
    plt.show()
    plt.close(fig)


def main():
    
    for pasta in PASTAS_TESTCASES:
        print(f"PASTA: {pasta}\n")
        
        test_cases = achar_testcases(pasta)

        if not test_cases:
            print(f"{pasta} sem arquivos válidos")
            continue

        dados_tempo, dados_operacoes, dados_memoria, dados_score, mapa_heuristica = (
            executar_benchmarks(test_cases)
        )

        gerar_graficos(dados_tempo, dados_operacoes, dados_memoria, dados_score, mapa_heuristica, pasta)


if __name__ == "__main__":
    main()