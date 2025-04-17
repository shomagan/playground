import osmnx as ox
import geopandas as gpd
import pandas as pd
import matplotlib.pyplot as plt
from shapely.geometry import Point



def plot_main(online=False, from_place=False):
    # Define the position (latitude, longitude)
        if from_place:
            road_types = [
                "motorway", "trunk", "primary", "secondary", "tertiary",  # Major roads
                "residential", "unclassified", "service", "living_street"]  # Minor roadsprint("get roads")
            print("get roads")    
            roads = ox.features.features_from_place('Krakow, Poland', tags={"highway": road_types})
            rivers = ox.features.features_from_place('Krakow, Poland', tags={"waterway": ["river", "stream"]})
            lakes = ox.features.features_from_place('Krakow, Poland', tags={"natural": "water"})

            # Plot everything
            fig, ax = plt.subplots(figsize=(10, 10))
            ax.set_axis_off()
            roads.plot(ax=ax, color="dimgrey", linewidth=0.25, label="Roads")
            rivers.plot(ax=ax, color="cornflowerblue", linewidth=0.5, label="Rivers")
            lakes.plot(ax=ax, color="cornflowerblue", alpha=0.5, label="Lakes")            
            plt.savefig("krakow.pdf")

            ax.set_title(f"over the garden wall")
            #ax.legend()
            plt.show()
        else:
            if online:
                lat = 50.06  # Example: Warsaw, Poland
                lon = 19.93
                print("get bike roads")
                bike_road_tags = {
                    "highway": ["cycleway", "path", "residential", "service"],
                    "bicycle": True  # Ensures roads explicitly marked as bike-friendly
                }
                bike_roads = ox.features.features_from_point([lat, lon], tags=bike_road_tags, dist=10000)
                # Get Roads
                road_types = [
                    "motorway", "trunk", "primary", "secondary", "tertiary",  # Major roads
                    "residential", "unclassified", "service", "living_street"]  # Minor roadsprint("get roads")
                print("get roads")    
                roads = ox.features.features_from_point([lat,lon], tags={"highway": road_types}, dist=10000)
                # Get Buildings
                print("get buildings")
                buildings = ox.features.features_from_point([lat,lon], tags={"building": True}, dist=10000)
                buildings = buildings[buildings.geometry.geom_type != "Point"]
                # Get leisure
                print("get leisure")
                leisure = ox.features.features_from_point([lat,lon], tags={"leisure": True}, dist=10000)
                leisure = leisure[leisure.geometry.geom_type != "Point"]
                # Get Rivers (Waterways)
                print("get rivers")
                rivers = ox.features.features_from_point([lat,lon], tags={"waterway": ["river", "stream"]}, dist=10000)
                # Get Lakes and caves(Water bodies)
                print("get lakes")
                lakes = ox.features.features_from_point([lat,lon], tags={"natural": "water"}, dist=10000)
                print("get caves")
                caves = ox.features.features_from_point([lat,lon], tags={"natural": "cave_entrance"}, dist=10000)
                # Get Climbing Areas or Crags
                #print("get climbing areas")
                #place_name = "Poland"
                #climbing_areas = ox.geometries_from_place(place_name, tags={"sport": "climbing"})
                data = bike_roads.to_json()
                # writing JSON data structure to a GeoJSON file
                with open("bike_roads.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                # converting GeoDataFrame to a JSON
                data = roads.to_json()
                # writing JSON data structure to a GeoJSON file
                with open("roads.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                data = buildings.to_json()
                with open("buildings.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                data = rivers.to_json()
                with open("rivers.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                data = lakes.to_json()
                with open("lakes.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                data = caves.to_json()
                with open("caves.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)

                data = leisure.to_json()
                with open("leisure.geojson", 'w', encoding='utf-8') as file:
                    file.write(data)
                #data = climbing_areas.to_json()
                #with open("climbing_areas.geojson", 'w', encoding='utf-8') as file:
                #    file.write(data)
            else:
                print("bike roads read")
                bike_roads = gpd.read_file('bike_roads.geojson')
                print("roads read")
                roads = gpd.read_file('roads.geojson')
                buildings = gpd.read_file('buildings.geojson')
                leisure = gpd.read_file('leisure.geojson')
                rivers = gpd.read_file('rivers.geojson')
                lakes = gpd.read_file('lakes.geojson')
                caves = gpd.read_file('caves.geojson')
                #climbing_areas = gpd.read_file('climbing_areas.geojson')
        
            # Plot everything
            fig, ax = plt.subplots(figsize=(10, 10))
            ax.set_axis_off()
            bike_roads.plot(ax=ax, color="blue", linewidth=0.15, label="bike_roads")    
            roads.plot(ax=ax, color="silver", linewidth=0.25, label="Roads")
            buildings.plot(ax=ax, color="chocolate", alpha=0.7, label="Buildings")
            leisure.plot(ax=ax, color="olive", alpha=0.7, label="leisure")
            rivers.plot(ax=ax, color="deepskyblue", linewidth=1, label="Rivers")
            lakes.plot(ax=ax, color="deepskyblue", alpha=0.5, label="Lakes")
            caves.plot(ax=ax, color="black", alpha=0.2, label="Caves")
            #climbing_areas.plot(ax=ax, color="silver", linewidth=0.5, label="Rocks")
            plt.savefig("krakow.pdf")
            ax.set_title(f"over the garden wall")
            #ax.legend()
            plt.show()

if __name__  == "__main__":
    plot_main(online=True,from_place=True)
