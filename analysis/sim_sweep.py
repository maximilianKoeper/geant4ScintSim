import json
import subprocess
import os

with open("../config/config.json") as f:
    data = json.load(f)

for angle in range(9):
    for dimZ in range(2, 42, 10):
        for dist in range(10, 50, 10):
            current_folder = "./output/" + str(10*angle) + "deg/" + str(dimZ) + "mm/" + str(dist) + "mm/"
            os.makedirs(current_folder, exist_ok = True)
            data["output_options"]["folder"] = current_folder
            data["geom_options"]["absorber_options"]["rotation"] = 10*angle
            data["geom_options"]["absorber_options"]["dimZ"] = dimZ
            data["geom_options"]["absorber_options"]["distance"] = dist
            data["geom_options"]["detector_dimensions"]["rotation"] = 10*angle
            with open("config_tmp.json", "w", encoding="utf-8") as f:
                json.dump(data, f, ensure_ascii=False, indent=4)

            p = subprocess.run("./Sim --mac run1.mac --config config_tmp.json", shell=True)
            if p.returncode != 0:
                os.remove(current_folder + "/output.json")
                os.rmdir(current_folder)