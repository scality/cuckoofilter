const assert = require('assert');

const CuckooFilter = require('../../index');

function key(i) {
    return `awesome-bucket-${i}`;
}

describe('CuckooFilter', () => {
    it('should store the target number of items without too many ' +
    'false positives', () => {
        const MAX_ITEMS = process.env.CUCKOOFILTER_TEST_MAX_ITEMS ?
              Number.parseInt(process.env.CUCKOOFILTER_TEST_MAX_ITEMS) : 1000000;
        const NB_RANGES = 10;

        const cf = new CuckooFilter(MAX_ITEMS);
        for (let q = 0; q < NB_RANGES; ++q) {
            const startAt = MAX_ITEMS * q / NB_RANGES;
            const stopAt = MAX_ITEMS * (q + 1) / NB_RANGES;
            for (let i = startAt; i < stopAt; ++i) {
                cf.add(key(i));
            }
            for (let i = startAt; i < stopAt; ++i) {
                assert.strictEqual(cf.contain(key(i)), true, key(i));
            }
            let fpCount = 0;
            for (let i = stopAt; i < stopAt + (stopAt - startAt); ++i) {
                if (cf.contain(key(i))) {
                    fpCount += 1;
                }
            }
            const fpRatio = fpCount / (stopAt - startAt);
            console.log(`    false positive ratio: ${fpCount}/${stopAt - startAt} (${fpRatio*100}%)`);
            // check that false positives stay below 1% (they should
            // normally be ~0.1%-0.3%)
            assert(fpRatio < 0.01,
                   `false positive ratio too high (${fpRatio * 100}%) for range ${q}`);
        }
    });
    it('should sustain continuous adds and deletes', () => {
        const MAX_ITEMS = process.env.CUCKOOFILTER_TEST_MAX_ITEMS ?
              Number.parseInt(process.env.CUCKOOFILTER_TEST_MAX_ITEMS) : 1000000;
        const NB_RANGES = 100;

        const cf = new CuckooFilter(MAX_ITEMS);
        const rangePresent = {};
        for (let q = 0; q < NB_RANGES; ++q) {
            rangePresent[q] = false;
        }
        function addRange(q) {
            assert.strictEqual(rangePresent[q], false);
            const startAt = MAX_ITEMS * q / NB_RANGES;
            const stopAt = MAX_ITEMS * (q + 1) / NB_RANGES;
            console.log(`adding range ${startAt}..${stopAt}`);
            for (let i = startAt; i < stopAt; ++i) {
                cf.add(key(i));
            }
            rangePresent[q] = true;
        }
        function removeRange(q) {
            assert.strictEqual(rangePresent[q], true);
            const startAt = MAX_ITEMS * q / NB_RANGES;
            const stopAt = MAX_ITEMS * (q + 1) / NB_RANGES;
            console.log(`removing range ${startAt}..${stopAt}`);
            for (let i = startAt; i < stopAt; ++i) {
                cf.delete(key(i));
            }
            rangePresent[q] = false;
        }
        function checkRange(q) {
            const startAt = MAX_ITEMS * q / NB_RANGES;
            const stopAt = MAX_ITEMS * (q + 1) / NB_RANGES;
            if (rangePresent[q]) {
                // check that there is no false negative
                for (let i = startAt; i < stopAt; ++i) {
                    assert.strictEqual(cf.contain(key(i)), true, key(i));
                }
            } else {
                let fpCount = 0;
                for (let i = startAt; i < stopAt; ++i) {
                    if (cf.contain(key(i))) {
                        fpCount += 1;
                    }
                }
                const fpRatio = fpCount / (stopAt - startAt);
                // check that false positives stay below 1% (they
                // should normally be ~0.1%-0.3%)
                console.log(`    false positive ratio: ${fpRatio * 100}%`);
                assert(fpRatio < 0.01,
                       `false positive ratio too high (${fpRatio * 100}%) for range ${q}`);
            }
        }
        for (let q = 0; q < NB_RANGES; ++q) {
            addRange(q);
            checkRange(q);
        }
        for (let q = NB_RANGES; q < 2 * NB_RANGES; ++q) {
            checkRange(q);
        }
        for (let q = 0; q < NB_RANGES; ++q) {
            if (q % 3 === 0) {
                removeRange(q);
            }
            checkRange(q);
        }
        for (let q = 0; q < NB_RANGES; ++q) {
            if (q % 3 === 0) {
                addRange(q);
            } else if (q % 3 === 1) {
                removeRange(q);
            }
            checkRange(q);
        }
        for (let q = 0; q < NB_RANGES; ++q) {
            if (q % 3 === 1) {
                addRange(q);
            } else if (q % 3 === 2) {
                removeRange(q);
            }
            checkRange(q);
        }
        for (let q = 0; q < NB_RANGES; ++q) {
            if (q % 3 === 2) {
                addRange(q);
            }
            checkRange(q);
        }
        for (let q = NB_RANGES; q < 2 * NB_RANGES; ++q) {
            checkRange(q);
        }
    });
});
