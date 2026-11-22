
import glob
import re
import subprocess

RODADAS = 10

with open("benchmark.txt", "w") as saida:
    for instancia in sorted(glob.glob("instances/*.tsp")):
        custos = []
        tempos = []

        for r in range(RODADAS):
            texto = subprocess.run(["./bnc", instancia],
                                   capture_output=True, text=True).stdout
            ub = re.search(r"^UB:\s*(\S+)", texto, re.MULTILINE)
            tempo = re.search(r"^Total_Time:\s*(\S+)", texto, re.MULTILINE)
            if ub and tempo:
                custos.append(float(ub.group(1)))
                tempos.append(float(tempo.group(1)))

        print(instancia)
        saida.write(instancia + "\n")

        if custos:
            media_custo = sum(custos) / len(custos)
            media_tempo = sum(tempos) / len(tempos)
            linha = f"{media_custo:g} {media_tempo:g}"
        else:
            linha = "erro"

        print(linha)
        saida.write(linha + "\n")

    saida.write("-\n")