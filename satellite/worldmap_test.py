import cartopy.crs as ccrs
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(1,1,1, projection=ccrs.PlateCarree())
ax.coastlines()
fig.savefig("test.jpg")
