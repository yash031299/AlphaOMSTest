# 📌 TODO — accounting-service

Tracks wallet debits/credits, funding PnL, realized PnL, and periodic ledger snapshots.

---

## ✅ Phase 1: Core Engine

- [x] Ledger system with mutex safety
- [x] FundingHandler (receive PnL from funding-engine)
- [x] PnLTracker (receive PnL from RMS/match-engine)
- [x] SettlementEngine (snapshot to CSV)

---

## ✅ Phase 2: Integration Hooks

- [x] Add gRPC input to receive funding and PnL events
- [ ] Optional Redis subscription (`accounting-events`)
- [x] Validate incoming data (basic structure accepted, replay/risk checks optional)

---

## ✅ Phase 3: Storage Output

- [x] Store snapshot to PostgreSQL
- [ ] Upload snapshots to S3 or persistent volume
- [ ] Add retention policy on files

---

## ✅ Phase 4: Observability

- [x] Prometheus metrics for ledger size, flush latency
- [x] gRPC/HTTP health check endpoint
- [x] Admin API to query user entries

---

## ❌ Phase 5: Testing (Pending)

- [ ] Unit test: ledger credit/debit
- [ ] Integration test: full settlement cycle
- [ ] Fuzz test with malformed funding data

---

## ✅ Phase 6: Readiness

- [x] Dockerfile with multi-stage build
- [x] Configurable via env vars (port, interval, redis toggle)
- [x] Signal handling with graceful shutdown

---

## 🧠 Notes

- Thread-safe, append-only ledger model
- Horizontally scalable with per-user sharding
- Works in real-time and batch mode
