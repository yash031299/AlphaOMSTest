# 📌 TODO — audit-log-service

This service handles append-only audit logs for compliance, debugging, and analytics.

---

## ✅ Phase 1: Core Logging (Completed)

- [x] `event_types.hpp`: unified event models (order, trade, funding, liquidation)
- [x] `log_writer`: JSON-per-line file logger with mutex protection
- [x] Console app logs to `data/logs/`
- [x] Meson build
- [x] Dockerfile image

---

## ✅ Phase 2: Integration & Input

- [x] Add gRPC `LogEventService` endpoint
- [x] Support Redis consumer (optional)
- [x] Use async buffered logger (ring queue)

---

## 🧠 Phase 3: Scalability

- [ ] Compress logs daily (gzip/zip)
- [ ] Rotate files weekly
- [ ] Upload logs to S3 or mounted volume (if enabled)
- [ ] Write batch insert support for PostgreSQL (optional)

---

## ⚙️ Phase 4: Monitoring & Observability

- [x] Log IOPS metrics (writes/sec)
- [ ] Prometheus metric: last write timestamp
- [ ] `/healthz` endpoint or gRPC health probe — _skipped for now_

---

## 🔬 Phase 5: Testing

- [ ] Unit test all `toJson()` functions
- [ ] Fuzz malformed inputs
- [ ] Inject concurrent writes to stress `LogWriter`

---

## 🧠 Notes

- This logger is **thread-safe, append-only, and horizontally scalable**
- Use for post-mortem forensics, compliance audits, PnL investigations
- Can be extended into a full analytics ETL pipeline (ClickHouse, ELK, etc)
