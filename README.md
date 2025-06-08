# Alpha OMS — Microservices Trading Engine

A high-performance order management system built in modern C++ with microservices, gRPC, Redis, and Clean Architecture.

## 🧩 Services

| Service           | Description                            |
| ----------------- | -------------------------------------- |
| order-service     | Receives and routes user orders        |
| execution-service | Executes LIMIT/MARKET/SL orders        |
| risk-service      | Monitors LTP, margin risk, liquidation |
| user-service      | Wallets, balance checks, scheduler     |
| symbol-service    | Orderbook & snapshot manager           |
| stream-service    | Redis tick stream consumer             |
| redis-service     | Publishes trade/order updates to Redis |
| config-service    | .env loader for all microservices      |

## 🧪 Testing

After build completion, use:

- `tools/test_order_cli` → to simulate CLI orders
- `order_service` → entry point
- Logs confirm full pipeline execution

## 🐳 Docker Compose (Coming)

```bash
docker-compose up --build
```
