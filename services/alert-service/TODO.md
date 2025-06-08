# 📌 TODO — alert-service

Monitors margin, PnL, balance, and raises alerts via log, Redis, webhook.

---

## ✅ Phase 1: Core Detection Loop

- [x] RuleEngine to load and evaluate JSON rules
- [x] Dispatcher logs triggered alerts
- [x] Boost Asio loop via AnomalyDetector

---

## 🔄 Phase 2: Integrations

- [ ] Connect to Redis stream or event-bus
- [ ] Read real RMS data (margin, pnl, balance)
- [ ] Push alerts to Redis topic (`alert-stream`)
- [ ] Optional: webhook / email / Slack

---

## 🛡️ Phase 3: Resilience

- [ ] Handle malformed or missing rules
- [ ] Retry on dispatch failure
- [ ] Healthcheck and crash recovery

---

## 📊 Phase 4: Observability

- [ ] Prometheus metrics:
  - alert_triggered_total
  - anomaly_detected_total
- [ ] Log rotation / alert audit database

---

## 🧪 Phase 5: Testing

- [ ] Unit test each alert rule
- [ ] Simulate 1000 user scan loop
- [ ] Validate no false positives

---

## 🧠 Notes

- All alerts currently printed to log
- Configurable alert rule JSON for non-devs
- Can later integrate PagerDuty, Opsgenie, or alert logs
