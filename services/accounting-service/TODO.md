# 📌 TODO — accounting-service

Tracks wallet debits/credits, funding PnL, realized PnL, and periodic ledger snapshots.

---

## ✅ Phase 1: Core Engine

- [x] Ledger system with mutex safety
- [x] FundingHandler (receive PnL from funding-engine)
- [x] PnLTracker (receive PnL from RMS/match-engine)
- [x] SettlementEngine (snapshot to CSV)

---

## 🔁 Phase 2: Integration Hooks

- [ ] Add gRPC input to receive funding and PnL events
- [ ] Optional Redis subscription (`accounting-events`)
- [ ] Validate incoming data (risk, replay protection)

---

## 📦 Phase 3: Storage Output

- [ ] Store snapshot to PostgreSQL
- [ ] Upload snapshots to S3 or persistent volume
- [ ] Add retention policy on files

---

## 📊 Phase 4: Observability

- [ ] Prometheus metrics for ledger size, flush latency
- [ ] gRPC/HTTP health check endpoint
- [ ] Admin API to query user entries

---

## 🧪 Phase 5: Testing

- [ ] Unit test: ledger credit/debit
- [ ] Integration test: full settlement cycle
- [ ] Fuzz test with malformed funding data

---

## 🧠 Notes

- Thread-safe, append-only model
- Easily horizontally scalable (per-user sharding)
- Works in real-time or batch mode
