# 📌 TODO — match-engine

This file tracks all pending work to productionize the match engine.

---

## ✅ Phase 1: Core Engine (Completed)

- [x] `OrderBook` with price-time FIFO matching
- [x] `Matcher` for symbol routing
- [x] `TradePublisher` to log trades
- [x] `main.cpp` to test order flow
- [x] Meson build
- [x] Dockerfile for deployment

---

## 🛠️ Phase 2: Feature Expansion

- [ ] Add modify order support
- [ ] Add order expiration or IOC/FOK flags
- [ ] Add order types: market, stop-limit

---

## 🧵 Phase 3: Concurrency & Sharding

- [x] Per-symbol isolation
- [ ] Thread pool for ingesting orders (from Kafka/Redis/gRPC)
- [ ] Lock-free queues (if latency critical)

---

## 🔒 Phase 4: Input & API Integration

- [ ] Redis stream listener for `order-stream`
- [ ] gRPC `SubmitOrder` and `CancelOrder` endpoints
- [ ] Auth token validation (if exposed externally)

---

## 📊 Phase 5: Observability

- [ ] Prometheus metrics:
  - Active orders
  - Match latency
  - Per-symbol throughput
- [ ] `/healthz` probe for readiness

---

## 🧪 Phase 6: Testing

- [ ] Unit test `OrderBook::processOrder()`
- [ ] Load test with synthetic burst orders
- [ ] Replay test from CSV market dump

---

## 🧠 Notes

- Current version is **single-threaded, per-symbol isolated, memory-efficient**
- Good for up to ~10K QPS per symbol
- Fully decoupled from external IO (ready for ingestion plug-in)
