| Policy | Best for | Weak against | Complexity | Notes |
|---|---|---|---|---|
| LRU | General workloads | Large scans | O(1) | Best default choice |
| LFU | Stable hotsets | Bursty traffic; stale popularity (no aging) | O(1)* |  Highest hit ratio on static skew; *O(1) requires frequency-bucket lists|
| ARC | Mixed/adaptive workloads | CPU overhead;  Extra metadata (ghost lists) | O(1) |  Self-tuning recency/frequency balance (two LRU lists + miss history) |
| FIFO | Simple queues | Locality-heavy access | O(1) | Lowest overhead |
| MRU | Sequential scans | Temporal locality | O(1) | Opposite of LRU |
| TTL | Expiring data | High churn | O(log n) | Time-based eviction, not capacity-based; usually layered over LRU/LFU|

## Which policy should I use?

- Use **LRU** if you don't know your workload.
- Use **LFU** for stable hotsets and popularity-based access.
- Use **ARC** when workloads change dynamically.
- Use **FIFO** for ultra-low overhead queues.
- Use **MRU** for scan-heavy workloads.
- Use **TTL** when correctness depends on expiration time.

| Policy | get | put | evict |
|---|---|---|---|
| FIFO | O(1) | O(1) | O(1) |
| LRU | O(1) | O(1) | O(1) |
| MRU | O(1) | O(1) | O(1) |
| LFU | O(1) | O(1) | O(1) |
| ARC | O(1) | O(1) | O(1) |
| TTL | O(log n) | O(log n) | O(log n) |

## Workload characteristics

| Workload | Best policy |
|---|---|
| Temporal locality | LRU |
| Stable popularity | LFU |
| Adaptive mixed load | ARC |
| Sequential scans | MRU |
| Queue-like buffering | FIFO |
| Expiring data | TTL |
