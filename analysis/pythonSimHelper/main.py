import matplotlib.pyplot as plt
import matplotlib.colors as colors

import numpy as np

##############################################################################################################################

def plotEvent(eventId: int, data: dict) -> np.ndarray:

    f_size = 10

    edep_data = np.zeros((80,80), dtype=float)

    for key in data["event"][str(eventId)]["tileHitEdep"]:
        x = (int(key)+1) % 80
        y = (int(key)+1) // 80
        edep_data[x][y] = data["event"][str(eventId)]["tileHitEdep"][key]

    fig, axs = plt.subplots()
    axs.set_aspect('equal', 'box')

    im_plot = axs.imshow(edep_data, cmap="magma_r")
    cbar = fig.colorbar(im_plot, ax=axs, fraction=0.04, pad=0.04)
    cbar.set_label("edep[MeV]")
    axs.set_title("Event: " + str(eventId) + " with energy: " + str(round(data["event"][str(eventId)]["particle_energy"], 3)) + " MeV", fontsize=f_size)
    fig.tight_layout()


    return edep_data

##############################################################################################################################

def plotObservables(data: dict):
    eparticle_data = np.array(data["eparticle"])
    edep_data = np.array(data["edep"])
    meanspred_data = data["meanspred"]
    spreadfrommaxedep_data = data["spreadfrommaxedep"]
    meanspred_data_x = data["meanspred_x"]
    meanspred_data_y = data["meanspred_y"]
    numberofhits_data = data["numberofhits"]


    #H_edep, x_edges_edep, y_edges_edep = np.histogram2d(eparticle_data, edep_data, bins=100)
    H_edep, x_edges_edep, y_edges_edep = np.histogram2d(eparticle_data, edep_data, bins=100)#, range=[[0,5000],[0, np.mean(edep_data)*3]])
    H_spreadFromMean, x_edges_SFMean, y_edges_SFMean = np.histogram2d(eparticle_data, meanspred_data, bins=100)
    H_spreadFromMax, x_edges_SFMax, y_edges_SFMax = np.histogram2d(eparticle_data, spreadfrommaxedep_data, bins=100)
    H_spreadX, x_edges_SX, y_edges_SX = np.histogram2d(eparticle_data, meanspred_data_x, bins=100)
    H_spreadY, x_edges_SY, y_edges_SY = np.histogram2d(eparticle_data, meanspred_data_y, bins=100)
    H_NHits, x_edges_NHits, y_edges_NHits = np.histogram2d(eparticle_data, numberofhits_data, bins=100)

    f_size = 10

    fig, axs = plt.subplots(3,2,figsize=(10, 10))

    ##############
    ax = axs[0][0]
    scatter_plot = ax.pcolormesh(x_edges_edep, y_edges_edep, H_edep.T, norm=colors.LogNorm(vmin=0.9, vmax=H_edep.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("Edep distribution", fontsize=f_size)
    ax.set_ylabel("edep in detector [MeV]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ##############
    ax = axs[0][1]
    scatter_plot = ax.pcolormesh(x_edges_SFMean, y_edges_SFMean, H_spreadFromMean.T, norm=colors.LogNorm(vmin=0.9, vmax=H_spreadFromMean.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("Spread from center of Energy", fontsize=f_size)
    ax.set_ylabel("~distance [A.U.]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ##############
    ax = axs[1][0]
    scatter_plot = ax.pcolormesh(x_edges_NHits, y_edges_NHits, H_NHits.T, norm=colors.LogNorm(vmin=0.9, vmax=H_spreadFromMean.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("NHits", fontsize=f_size)
    ax.set_ylabel("NHits [#]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ##############
    ax = axs[1][1]
    scatter_plot = ax.pcolormesh(x_edges_SFMax, y_edges_SFMax, H_spreadFromMax.T, norm=colors.LogNorm(vmin=0.9, vmax=H_spreadFromMean.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("Spread from max Energy", fontsize=f_size)
    ax.set_ylabel("~distance [A.U.]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ##############
    ax = axs[2][0]
    scatter_plot = ax.pcolormesh(x_edges_SX, y_edges_SX, H_spreadX.T, norm=colors.LogNorm(vmin=0.9, vmax=H_spreadFromMean.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("Spread in x direction", fontsize=f_size)
    ax.set_ylabel("~distance [A.U.]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ##############
    ax = axs[2][1]
    scatter_plot = ax.pcolormesh(x_edges_SY, y_edges_SY, H_spreadY.T, norm=colors.LogNorm(vmin=0.9, vmax=H_spreadFromMean.max()), cmap='rainbow')
    cbar = fig.colorbar(scatter_plot, ax=ax, fraction=0.035, pad=0.04)
    cbar.set_label("#events")

    ax.set_title("Spread in y direction", fontsize=f_size)
    ax.set_ylabel("~distance [A.U.]", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    fig.tight_layout()
    plt.show()


########################################################################################################################

def calcObservables(data: dict) -> dict:
    eparticle_data = []
    edep_data = []
    meanspred_data = []
    spreadfrommaxedep_data = []
    meanspred_data_x = []
    meanspred_data_y = []
    numberofhits_data=[]

    counter_no_energy = 0
    i = 0

    for eventid in data["event"]:
        if i > 10000:
            break
        i += 1
        edep_dist_tmp = np.zeros((80,80), dtype=float)
        if "tileHitEdep" not in data["event"][str(eventid)].keys():
            counter_no_energy += 1
            # print("Event " + str(eventid) + " has no energy deposit")
            continue
        #if data["event"][str(eventid)]["edep_detector"] <= 10:
        #    continue        
        
        # calculating energy spread
        x_arr_tmp = []
        y_arr_tmp = []
        
        e_arr_tmp = []
        index = np.array([0,0])
        if "tileHitEdep" in data["event"][str(eventid)].keys():
            for key in data["event"][str(eventid)]["tileHitEdep"]:

                edep = data["event"][str(eventid)]["tileHitEdep"][key]

                x = (int(key)) % 80
                y = (int(key)) // 80

                x_arr_tmp.append(x)
                y_arr_tmp.append(y)
                e_arr_tmp.append(edep)

                index[0] += x*edep
                index[1] += y*edep  
                
                edep_dist_tmp[x][y] = edep


        index_mean = np.round(index/data["event"][str(eventid)]["edep_detector"])
        index_max = np.unravel_index(edep_dist_tmp.argmax(), edep_dist_tmp.shape)

        spread_from_mean = 0.
        spread_x = 0.
        spread_y = 0.
        spread_from_max = 0.
        
        #e_mean_tmp = np.mean(e_arr_tmp)
        e_sum = data["event"][str(eventid)]["edep_detector"]
        for entry in range(len(e_arr_tmp)):
            spread_from_mean += np.sqrt((x_arr_tmp[entry]-index_mean[0])**2 +(y_arr_tmp[entry]-index_mean[1])**2)*(e_arr_tmp[entry]/e_sum)
            spread_from_max += np.sqrt((x_arr_tmp[entry]-index_max[0])**2 +(y_arr_tmp[entry]-index_max[1])**2)*(e_arr_tmp[entry]/e_sum)
            spread_x += abs((x_arr_tmp[entry]-index_max[0]))*(e_arr_tmp[entry]/e_sum)
            spread_y += abs((y_arr_tmp[entry]-index_max[1]))*(e_arr_tmp[entry]/e_sum)
            

        edep_data.append(data["event"][str(eventid)]["edep_detector"])
        eparticle_data.append(data["event"][str(eventid)]["particle_energy"])
        meanspred_data.append(spread_from_mean)
        spreadfrommaxedep_data.append(spread_from_max)
        meanspred_data_x.append(spread_x)
        meanspred_data_y.append(spread_y)
        numberofhits_data.append(len(e_arr_tmp))

        return_dict = {"edep": edep_data, "eparticle": eparticle_data, "meanspred": meanspred_data, "spreadfrommaxedep": spreadfrommaxedep_data, "meanspred_x": meanspred_data_x, "meanspred_y": meanspred_data_y, "numberofhits": numberofhits_data}

    print("Number of events with no energy deposition: ", counter_no_energy)

    return return_dict

########################################################################################################################

def calcObservablesReduced(data: dict) -> dict:
    eparticle_data = []
    edep_data = []
    numberofhits_data=[]

    counter_no_energy = 0

    for eventid in data["event"]:
        if "tileHitEdep" not in data["event"][str(eventid)].keys():
            counter_no_energy += 1
            # print("Event " + str(eventid) + " has no energy deposit")
            continue            

        edep_data.append(data["event"][str(eventid)]["edep_detector"])
        eparticle_data.append(data["event"][str(eventid)]["particle_energy"])
        numberofhits_data.append(len(data["event"][str(eventid)]["tileHitEdep"]))

        return_dict = {"edep": edep_data, "eparticle": eparticle_data, "numberofhits": numberofhits_data}

    print("Number of events with no energy deposition: ", counter_no_energy)

    return return_dict

def plotSigmaOverObs(data: dict) -> dict:
    eparticle_data = np.array(data["eparticle"])
    edep_data = np.array(data["edep"])
    numberofhits_data = np.array(data["numberofhits"])


    H_edep, x_edges_edep, y_edges_edep = np.histogram2d(eparticle_data, edep_data, bins=100, range=[[0,5000],[0, np.mean(edep_data)*3]])
    H_NHits, x_edges_NHits, y_edges_NHits = np.histogram2d(eparticle_data, numberofhits_data, bins=100, range=[[0,5000],[0, np.mean(numberofhits_data)*3]])

    ycenters = (y_edges_edep[:-1] + y_edges_edep[1:]) / 2
    xcenters = (x_edges_edep[:-1] + x_edges_edep[1:]) / 2

    ycenters_NH = (y_edges_NHits[:-1] + y_edges_NHits[1:]) / 2
    xcenters_NH = (x_edges_NHits[:-1] + x_edges_NHits[1:]) / 2

    sigmaOEnergy = []
    for i in range(len(H_edep.T[0])):
        mean = np.average(ycenters, weights=H_edep[i])
        var = np.average((ycenters - mean)**2, weights=H_edep[i])
        sig = np.sqrt(var)
        
        sigOEnergy = sig/sum(H_edep[i])
        sigmaOEnergy.append(sigOEnergy)
        
    sigmaONHits = []
    for i in range(len(H_NHits.T[0])):
        mean = np.average(ycenters_NH, weights=H_NHits[i])
        var = np.average((ycenters_NH - mean)**2, weights=H_NHits[i])
        sig = np.sqrt(var)
        
        sigOHits = sig/sum(H_NHits[i])
        sigmaONHits.append(sigOHits)
        
    f_size = 15

    fig, axs = plt.subplots(2, figsize=(8, 8))

    ##############
    ax = axs[0]
    plot = ax.plot(xcenters, sigmaOEnergy)

    ax.set_title("$\sigma_E$ over $E_{dep}$", fontsize=f_size)
    ax.set_ylabel("$\sigma_E / E$", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)

    ax = axs[1]
    plot = ax.plot(xcenters_NH, sigmaONHits)

    ax.set_title("sigma_N over N", fontsize=f_size)
    ax.set_ylabel("$\sigma_N / N$", fontsize=f_size)
    ax.set_xlabel("particle energy [MeV]", fontsize=f_size)
    plt.tight_layout()

    plt.show()
