# vMaNGOS `mangosd` monitoring via metrics in Grafana

This folder contains a sample setup for vMaNGOS' monitoring stack.

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
1. `cd` into this folder
2. `docker compose down`
