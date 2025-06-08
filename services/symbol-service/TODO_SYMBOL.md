# 📌 TODO — symbol-service

This file tracks all pending enhancements, scalability upgrades, and production-grade features for the in-memory snapshot service.

---

## ✅ Phase 1: Core Engine (DONE)

- [x] Implement `OrderBookManager` with thread-safe per-symbol books
- [x] Support add, cancel, and modify orders
- [x] Generate Level-1 snapshots using `SnapshotGenerator`
- [x] Periodically log snapshots in `main.cpp`
- [x] Add health check logging
- [x] Dockerize with clean build
- [x] Meson build integration

---

## 🔄 Phase 2: Broadcast & Access

- [ ] Push Level-1 snapshots to Redis stream or pub/sub
- [ ] Optionally serve snapshots via gRPC `GetL1(symbol)`
- [ ] Add CLI/cron-based snapshot export (for audit/testing)

---

## 📊 Phase 3: Observability

- [ ] Add Prometheus metrics:
  - Total active orders
  - Top-of-book latency
  - Order volume per symbol
- [ ] Add `/healthz` HTTP endpoint for Kubernetes probe

---

## 🚨 Phase 4: Fault Tolerance

- [ ] Handle malformed symbol/order inputs gracefully
- [ ] Support backup snapshot restore on crash
- [ ] Add logging for invalid order transitions

---

## 🚀 Phase 5: Scalability Enhancements

- [ ] Use lock striping or per-symbol shard pools
- [ ] Auto-delete inactive symbol books
- [ ] Optimize book memory layout for SIMD/read-friendly access

---

## 🧪 Phase 6: Testing

- [ ] Unit test `OrderBookManager` with simulated activity
- [ ] Validate snapshot correctness with randomized data
- [ ] Fuzz test input paths

---

✅ Current version is clean, thread-safe, and scalable.
This TODO covers what’s next before production launch.
