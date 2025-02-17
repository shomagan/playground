import osmnx as ox
import geopandas as gpd
import pandas as pd
import matplotlib.pyplot as plt
from shapely.geometry import Point

# Define the position (latitude, longitude)
lat = 50.04  # Example: Warsaw, Poland
lon = 19.99
# Get Roads
road_types = [
    "motorway", "trunk", "primary", "secondary", "tertiary",  # Major roads
    "residential", "unclassified", "service", "living_street"]  # Minor roadsprint("get roads")
roads = ox.features.features_from_point([lat,lon], tags={"highway": road_types}, dist=1000)
# Get Buildings
print("get buildings")
buildings = ox.features.features_from_point([lat,lon], tags={"building": True}, dist=1000)
buildings = buildings[buildings.geometry.geom_type != "Point"]
# Get leisure
print("get leisure")
leisure = ox.features.features_from_point([lat,lon], tags={"leisure": True}, dist=1000)
leisure = leisure[leisure.geometry.geom_type != "Point"]
# Get Rivers (Waterways)
print("get rivers")
rivers = ox.features.features_from_point([lat,lon], tags={"waterway": ["river", "stream"]}, dist=2000)
# Get Lakes (Water bodies)
print("get lakes")
lakes = ox.features.features_from_point([lat,lon], tags={"natural": "water"}, dist=2000)
# converting GeoDataFrame to a JSON
data = roads.to_json()
# writing JSON data structure to a GeoJSON file
with open("roads.geojson", 'w', encoding='utf-8') as file:
    file.write(data)
data = buildings.to_json()
# writing JSON data structure to a GeoJSON file
with open("buildings.geojson", 'w', encoding='utf-8') as file:
    file.write(data)
data = rivers.to_json()
# writing JSON data structure to a GeoJSON file
with open("rivers.geojson", 'w', encoding='utf-8') as file:
    file.write(data)
data = lakes.to_json()
# writing JSON data structure to a GeoJSON file
with open("lakes.geojson", 'w', encoding='utf-8') as file:
    file.write(data)
data = leisure.to_json()
# writing JSON data structure to a GeoJSON file
with open("leisure.geojson", 'w', encoding='utf-8') as file:
    file.write(data)

# Plot everything
fig, ax = plt.subplots(figsize=(10, 10))
ax.set_axis_off()
roads.plot(ax=ax, color="silver", linewidth=0.5, label="Roads")
buildings.plot(ax=ax, color="chocolate", alpha=0.7, label="Buildings")
leisure.plot(ax=ax, color="olive", alpha=0.7, label="leisure")
rivers.plot(ax=ax, color="deepskyblue", linewidth=1, label="Rivers")
lakes.plot(ax=ax, color="deepskyblue", alpha=0.5, label="Lakes")

ax.set_title(f"over the garden wall")
#ax.legend()
plt.show()
