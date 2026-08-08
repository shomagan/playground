# listen_server

UDP broadcast listener that discovers devices on the LAN and reads/writes their
registers over HTTP, based on `sources/devices_description.json`.

## Build the image

```sh
docker build -t listen_server .
```

## Run

UDP broadcast discovery (port 65500) needs the container on the host's network,
so `--network host` is required (Linux only):

```sh
docker run -d --network host --name listen_server listen_server
```

## View logs

```sh
docker logs -f listen_server
```

## Stop

The app handles `SIGTERM` and shuts down cleanly:

```sh
docker stop listen_server
```

## Remove the container

```sh
docker rm listen_server
```

## Persist logged data

Register reads are logged as CSV files under `/app/data_log` inside the
container, one file per `device_name_mdbaddress_varname.csv`.

To have the CSVs land directly on the host (recommended — no copying needed):

```sh
docker run -d --network host --name listen_server \
  -v $(pwd)/data_log:/app/data_log \
  listen_server
```

Or, if using a named volume to keep data across restarts without a host path:

```sh
docker run -d --network host --name listen_server \
  -v listen_server_data:/app/data_log \
  listen_server
```

Pull the CSVs out of a named volume (or any running/stopped container) with:

```sh
docker cp listen_server:/app/data_log ./data_log
```

## Options

The binary accepts:

- `-d <path>` — device description JSON (defaults to `devices_description.json`
  in the working directory; the image's `CMD` points this at
  `/app/sources/devices_description.json`)
- `-l <level>` — log level: `DEBUG`, `INFO` (default), `WARNING`, `ERROR`

To override, pass them after the image name:

```sh
docker run -d --network host --name listen_server \
  listen_server -d /app/sources/devices_description.json -l DEBUG
```

## Use your own device description

Mount a custom description file over the default:

```sh
docker run -d --network host --name listen_server \
  -v /path/to/your_devices.json:/app/sources/devices_description.json:ro \
  listen_server
```

The app polls this file every 5 seconds and picks up changes without a restart.
