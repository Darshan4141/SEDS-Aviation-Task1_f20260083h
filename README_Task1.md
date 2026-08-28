# SEDS Avionics Task 1

## Finding the Sea Floor

This task focuses on processing depth data collected by the vehicle and using it to obtain a clearer representation of the sea floor.

The code first loads the data from the CSV file using Pandas and examines the depth measurements. The data is then cleaned to handle irregular readings and smoothed using a **rolling median filter**. This reduces the effect of sudden spikes and noise while preserving the overall shape of the depth profile.

The processed data is plotted using Matplotlib to show how the depth changes as the vehicle moves along its path. A **2D animation** is also generated to visualise the sea-floor profile as the vehicle progresses through the collected data.

## Files

* `SEDS_Avionics_Task1.ipynb` — Contains the Python code for loading, cleaning, processing and visualising the depth data.
* `Depth Data.csv` — Contains the depth measurements used for the analysis.
* `README.md` — Contains a brief explanation of the code and its functioning.
