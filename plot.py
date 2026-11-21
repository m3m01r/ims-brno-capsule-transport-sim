#
# IMS project
#
# Brief: script for processing simulation output and plot graphs
#
# Author: Albert Tikaiev (xtikaia00)
# Date: 2025-12-07
import os
from typing import Dict, List, Tuple
import matplotlib.pyplot as plt
import numpy as np
import sys


def plot_journeys(from_stop: str):
    """
    Inspired by:
    https://matplotlib.org/stable/gallery/lines_bars_and_markers/barchart.html#sphx-glr-gallery-lines-bars-and-markers-barchart-py
    """
    with open("./journeys.out") as f:
        content = f.read()
        lines = content.splitlines()
        stops: List[str] = []
        times: Dict[str, List[float]] = {
            'Mean': [],
            'Max': [],
            'Min': [],
        }
        for line in lines:
            splitted = line.split(" ")
            if splitted[0] == from_stop:
                stops.append(splitted[1])
                times['Mean'].append(np.floor(float(splitted[2])))
                times['Max'].append(np.floor(float(splitted[3])))
                times['Min'].append(np.floor(float(splitted[4])))

        width = 0.3
        multiplier = 0
        x = np.arange(len(stops))
        fig, ax = plt.subplots()
        fig.set_size_inches(16, 6)
        for desc, value in times.items():
            offset = width * multiplier
            rects = ax.bar(x + offset, value, width, label=desc)
            ax.bar_label(rects, padding=3)
            multiplier += 1

        ax.set_ylabel('Time (s)')
        ax.set_title(f"Time statistics for journeys from {from_stop}")
        ax.set_xticks(x + width, stops)
        fig.legend(loc='upper left', ncols=3)

        fig.tight_layout()
        fig.savefig("journeys.png")


def plot_waiting(files: List[Tuple[str, int]],
                 title: str,
                 xaxis_label: str,
                 yaxis_label: str,
                 variable: str,
                 save_file: str = "waiting.png"):
    x: List[int] = []
    y: List[float] = []
    latex: List[str] = []

    with open(files[0][0]) as f:
        content = f.read()
        lines = content.splitlines()
        latex.append(f"{variable} ")
        for line in lines:
            splitted = line.split(" ")
            latex.append(f"{splitted[0]} ")

    for (filename, count) in files:
        latex[0] = latex[0] + f" & {count}"
        with open(filename) as f:
            content = f.read()
            lines = content.splitlines()
            for (i, line) in enumerate(lines, start=1):
                splitted = line.split(" ")
                value = float(splitted[1])
                latex[i] = latex[i] + f"& {value:.1f} "
            total = sum([int(line.split(" ")[4]) for line in lines])
            means = sum([float(line.split(" ")[1]) *
                        int(line.split(" ")[4]) for line in lines])
            x.append(count)
            y.append(means / total)

    for line in latex:
        print(f"{line} \\\\ \\hline")

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.plot(x, y, '.-')
    ax.set_xticks(x)
    ax.get_xaxis().set_label_text(xaxis_label)
    ax.get_yaxis().set_label_text(yaxis_label)
    ax.set_title(title)
    ax.grid()
    fig.tight_layout()
    fig.savefig(save_file)


def plot_acceleration(files: List[Tuple[str, int]],
                      from_stop: str, to_stop: str,
                      save_file: str = "acceleration.png"):
    x: List[int] = []
    y: List[float] = []
    for (filename, acc) in files:
        with open(filename) as f:
            content = f.read()
            lines = content.splitlines()
            for line in lines:
                splitted = line.split(" ")
                if splitted[0] == from_stop and splitted[1] == to_stop:
                    x.append(acc)
                    y.append(np.floor(float(splitted[2])))

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.plot(x, y, '.-')
    ax.set_xticks(x)
    ax.grid()
    ax.get_xaxis().set_label_text("Acceleation (m/s^2)")
    ax.get_yaxis().set_label_text("Mean time (s)")
    ax.set_title(f"Mean travel time from {from_stop} to {
                 to_stop} based on acceleration")
    fig.tight_layout()
    fig.savefig(save_file)


if __name__ == "__main__":
    [(f"./{file}", int(file.split("_")[0]))
     for file in os.listdir(".") if file.endswith("_enter.out")]
    for arg in sys.argv:
        match arg:
            case '-c':
                plot_waiting(files=[(f"./{file}", int(file.split("_")[0]))
                                    for file in os.listdir(".") if file.endswith("_enter.out")],
                             title="Mean time that passengers wait before they can get into capsule",
                             xaxis_label="Number of capsules", yaxis_label="Mean time (s)",
                             variable="Capsules", save_file="enter_waiting.png"
                             )
            case '-l':
                plot_waiting(
                    files=[(f"./{file}", int(file.split("_")[0]))
                           for file in os.listdir(".") if file.endswith("_enter.out")],
                    title="Mean time that passengers wait for an available landing place at their destination stop",
                    xaxis_label="Number of landing places", yaxis_label="Mean time (s)",
                    variable="Landing places", save_file="depart_waiting.png"
                )
                break
            case '-a':
                plot_acceleration(
                    files=[(f"./{file}", int(file.split("_")[0]))
                           for file in os.listdir(".") if file.endswith("_journeys.out")],
                    from_stop="Reckovice", to_stop="Pionyrska")
                break
            case '-j':
                # plot_journeys("Jungmannova")
                plot_journeys("Reckovice")
                break
