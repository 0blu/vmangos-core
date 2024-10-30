# vMaNGOS `mangosd` monitoring via metrics in Grafana

This folder contains a sample setup for vMaNGOS' monitoring stack.  
It creates private GraphiteDB and public Grafana instance.  
It can handle metrics from multiple mangosd servers.

The database and Grafana dashboard are pre-configured for the default prefix `vmangos_metric`  
(see `Metric.ConnectionInfo` in `mangosd.conf`).

## Requirements
- Linux or [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
- `docker compose`
- Build `mangosd` with `-DENABLE_METRICS=On`

## Install
1. Copy this folder somewhere out of this repository and `cd` into it
2. Start docker compose script with: `docker compose up -d`
3. Verify status with: `docker compose ps`
4. Visit Grafana http://127.0.0.1:3580/
5. (You might want to keep the port behind a firewall)

## Uninstall
1. `cd` into the folder
2. `docker compose down`
