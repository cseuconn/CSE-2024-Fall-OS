import random

cache_size = 5
cache = set()

hits = 0
misses = 0

pages = "76004644413581538397602223208881788190703374313004"

for i in range(len(pages)):

    if int(pages[i]) in cache:
        hits += 1

    elif len(cache) < cache_size:
        cache.add(int(pages[i]))

    elif int(pages[i]) not in cache:
        misses += 1
        random_element = random.choice(list(cache))
        cache.remove(random_element)
        cache.add(int(pages[i]))

    print(cache)

print(f"Total Requests: {len(pages)}")
print(f"Hits: {hits}")
print(f"Misses: {misses}")

