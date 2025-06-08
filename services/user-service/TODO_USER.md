# ✅ TODO — user-service

Tracks all enhancements for scalability, observability, and fault-tolerance

---

## Phase 1: Functionality

- [ ] Add gRPC server (UserService) with methods:
  - GetWallet(userId)
  - ApplyMargin(userId, delta)
  - GetUserMetadata(userId)
- [ ] Add CLI for testing Redis margin messages

## Phase 2: Observability

- [ ] Add Prometheus metrics:
  - Wallet flush count
  - Total margin updates processed
  - Redis connection status
- [ ] Expose `/metrics` endpoint

## Phase 3: Resilience

- [ ] Add retry + backoff to RedisSafeClient
- [ ] Cache failed margin updates temporarily (if Redis is down)

## Phase 4: Scale

- [ ] Replace global mutex with lock striping (sharded maps)
- [ ] Move to async hiredis or use Boost Asio
- [ ] Horizontal scaling with gRPC load-balancing
