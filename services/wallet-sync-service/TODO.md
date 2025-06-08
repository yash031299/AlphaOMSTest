# ✅ TODO — wallet-sync-service

Tracks all planned tasks and enhancements.

---

## ✅ Phase 1: Core Sync Logic

- [x] Subscribe to Redis `trades` channel
- [x] Parse userId and unrealizedPnL from JSON
- [x] Apply PnL to Redis wallet state
- [x] Health endpoint `/healthz`

---

## 🔁 Phase 2: Production Hardening

- [ ] TLS connection to Redis
- [ ] Retry on wallet update failure
- [ ] Prometheus metrics (update latency, success/fail)
- [ ] Unit tests: `WalletStore::applyTrade`

---

## 🧠 Notes

- Wallet key: `wallet:<userId>` updated with HINCRBYFLOAT
- Stateless, restart-safe design
- Horizontally scalable (idempotent updates)
