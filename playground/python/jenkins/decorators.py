"""Reusable decorators: @retry(times, delay, exceptions) and @timed."""
import functools
import logging
import time

log = logging.getLogger(__name__)


def retry(times=3, delay=0.5, exceptions=(Exception,)):
    """Call the function up to `times` times in total, sleeping `delay` s between attempts.

    Only exceptions listed in `exceptions` trigger a retry; anything else propagates
    immediately. After the last failed attempt the original exception is re-raised.

    retry(...) is a decorator *factory*: it returns the real decorator, which is why
    it is used with parentheses: @retry() or @retry(times=5).
    """
    if times < 1:
        raise ValueError("times must be >= 1")

    def decorator(func):
        @functools.wraps(func)          # keep func.__name__ / __doc__ on the wrapper
        def wrapper(*args, **kwargs):
            for attempt in range(1, times + 1):
                try:
                    return func(*args, **kwargs)
                except exceptions as e:
                    if attempt == times:
                        raise           # bare raise keeps the original traceback
                    log.warning("%s: attempt %d/%d failed (%r), retry in %.2f s",
                                func.__qualname__, attempt, times, e, delay)
                    time.sleep(delay)
        return wrapper
    return decorator


def timed(func):
    """Log how long each call took (also when the call raises)."""
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()     # monotonic, high resolution; not time.time()
        try:
            return func(*args, **kwargs)
        finally:
            log.info("%s took %.1f ms", func.__qualname__,
                     (time.perf_counter() - start) * 1000)
    return wrapper


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format="%(levelname)s %(message)s")

    calls = 0

    @timed
    @retry(times=3, delay=0.1, exceptions=(ConnectionError,))
    def flaky():
        """Fails twice, then succeeds."""
        global calls
        calls += 1
        if calls < 3:
            raise ConnectionError("link down")
        return "ok"

    assert flaky() == "ok" and calls == 3
    assert flaky.__name__ == "flaky" and flaky.__doc__ == "Fails twice, then succeeds."

    @retry(times=2, delay=0)
    def always_fails():
        raise TimeoutError("no answer")

    try:
        always_fails()
    except TimeoutError:
        pass
    else:
        raise AssertionError("expected TimeoutError")

    @retry(times=5, delay=0, exceptions=(ConnectionError,))
    def wrong_type():
        raise KeyError("not retried")

    try:
        wrong_type()
    except KeyError:
        pass
    print("self-test passed")
