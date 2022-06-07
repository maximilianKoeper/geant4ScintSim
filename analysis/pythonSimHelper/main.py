import matplotlib.pyplot as plt


def plotEdepDistr(eventID: int, data: dict):
    f_size = 10

    x_data = []
    y_data = []
    edep_data = []

    for key in data["event"][str(eventID)]["tileHitEdep"]:
        x_data.append(data["geometry"]["tiles"][key]["posX"])
        y_data.append(data["geometry"]["tiles"][key]["posY"])
        edep_data.append(data["event"][str(eventID)]["tileHitEdep"][key])

    fig, axs = plt.subplots()
    axs.set_aspect('equal', 'box')
    scatter_plot = axs.scatter(x_data, y_data, c=edep_data, cmap="magma_r")
    cbar = fig.colorbar(scatter_plot, ax=axs, fraction=0.04, pad=0.04)
    cbar.set_label("edep[MeV]")

    axs.set_title("Edep distribution", fontsize=f_size)
    axs.set_ylabel("y [mm]", fontsize=f_size)
    axs.set_xlabel("x [mm]", fontsize=f_size)

    fig.tight_layout()
    plt.show()
