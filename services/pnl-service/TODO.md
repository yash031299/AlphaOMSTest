# 📌 TODO — pnl-service

This file tracks pending work to make `pnl-service` production-ready and scalable.

---

## ✅ Phase 1: Core Logic

- [x] `PnLEngine`: Track entry, quantity, and LTP
- [x] Compute unrealized PnL and equity
- [x] `EquityCalculator`: Wrapper for symbol-based liquidation checks
- [x] gRPC Server with:
  - `GetEquity(userId)`
  - `CheckLiquidation(userId)`
- [x] gRPC Client for symbol-registry-service
- [x] Build + run via Meson
- [x] Dockerfile for containerization

---

## 🛠️ Phase 2: Extensions

- [ ] Add gRPC endpoint: `GetAllPnL(userId)`
- [ ] Add `updateWallet(userId, delta)` RPC
- [ ] Track realized PnL separately (e.g., in file/db)
- [ ] Support funding rate impact (if perpetuals)

---

## 🔒 Phase 3: Fault Tolerance

- [ ] Add circuit breaker to gRPC symbol client
- [ ] Graceful fallback for missing metadata
- [ ] Recover state from disk if restarted

---

## 📊 Phase 4: Monitoring

- [ ] Prometheus metrics:
  - RPC latency
  - Failed symbol lookups
  - Equity volatility per user
- [ ] `/healthz` HTTP probe for liveness

---

## 🧪 Phase 5: Testing

- [ ] Unit tests for:
  - `PnLEngine`
  - `EquityCalculator`
- [ ] gRPC integration test using Python client
- [ ] Load test for 1000 users

---

## 🧠 Notes

- This service is stateless and horizontally scalable.
- Relies on in-memory maps for users — good for 1K-10K users; needs Redis/DB for long-term scale.
- Liquidation threshold currently hardcoded via symbol-registry margin values.
