import osmnx as ox
import geopandas as gpd
import matplotlib.pyplot as plt
from shapely.geometry import Point

# Define the position (latitude, longitude)
lat = 50.04  # Example: Warsaw, Poland
lon = 19.99
# Get Roads
road_types = [
    "motorway", "trunk", "primary", "secondary", "tertiary",  # Major roads
    "residential", "unclassified", "service", "living_street"]  # Minor roadsprint("get roads")
roads = ox.features.features_from_point([lat,lon], tags={"highway": road_types}, dist=2000)
# Get Buildings
print("get buildings")
buildings = ox.features.features_from_point([lat,lon], tags={"building": True}, dist=2000)
buildings = buildings[buildings.geometry.geom_type != "Point"]
# Get Rivers (Waterways)
print("get rivers")
rivers = ox.features.features_from_point([lat,lon], tags={"waterway": ["river", "stream"]}, dist=2000)
# Get Lakes (Water bodies)
print("get lakes")
lakes = ox.features.features_from_point([lat,lon], tags={"natural": "water"}, dist=2000)

roads.to_file("poland_roads.geojson", driver="GeoJSON")
buildings.to_file("poland_buildings.geojson", driver="GeoJSON")
rivers.to_file("poland_rivers.geojson", driver="GeoJSON")
lakes.to_file("poland_lakes.geojson", driver="GeoJSON")

# Plot everything
fig, ax = plt.subplots(figsize=(10, 10))
roads.plot(ax=ax, color="black", linewidth=0.5, label="Roads")
buildings.plot(ax=ax, color="gray", alpha=0.7, label="Buildings")
rivers.plot(ax=ax, color="blue", linewidth=1, label="Rivers")
lakes.plot(ax=ax, color="cyan", alpha=0.5, label="Lakes")

ax.set_title(f"OSM Data of Poland: Roads, Buildings, Rivers & Lakes")
ax.legend()
plt.show()
