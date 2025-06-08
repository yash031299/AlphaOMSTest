# 📌 TODO — order-service Microservice

This file tracks all tasks needed to make `order-service` production-ready, scalable, and maintainable.

---

## ✅ Phase 1: Core Enrichments

- [ ] Add `symbol-client.cpp` to fetch symbol metadata (tick size, leverage, precision).
- [ ] Add `user-client.cpp` to fetch user metadata (KYC status, role, balance).
- [ ] Validate symbol existence via gRPC.
- [ ] Validate user permissions, risk role.
- [ ] Enrich order before sending to execution (rounding, leverage checks).

---

## 🛡️ Phase 2: Security & Observability

- [ ] Enable TLS/mTLS for gRPC endpoints.
- [ ] Add Prometheus metrics: total routed, latency, error rate.
- [ ] Implement gRPC health checks for container readiness/liveness.
- [ ] Add rate limiting (optionally via API gateway or interceptor).

---

## ⚙️ Phase 3: Fault Tolerance

- [ ] Add retry mechanism for failed execution RPCs.
- [ ] Add timeout and cancellation support to gRPC client.
- [ ] Log failed orders for audit/debugging.
- [ ] Add Redis/Kafka failover queue (optional for high availability).

---

## 🔎 Phase 4: Testing

- [ ] Unit test `Validator`, `OrderRouter`, `ExecutionClient`.
- [ ] Integration tests with symbol- and user-service gRPC stubs.
- [ ] Load test `RouteOrder` using 1000+ QPS.
- [ ] Fuzz testing for invalid/malformed inputs.

---

## 🧠 Notes

- This service is currently stateless and thread-safe.
- Ready for horizontal scaling via Kubernetes or Docker.
- gRPC-based execution ensures decoupled architecture.
- Future: integrate Redis or Kafka as fallback channels.
