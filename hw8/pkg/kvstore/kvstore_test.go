/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package kvstore

import (
	"io/ioutil"
	"testing"

	"github.com/stretchr/testify/assert"
)

const (
	testKey    = "key"
	testValue1 = "value1"
	testValue2 = "value2"
)

func TestFSKVStore(t *testing.T) {
	fskv, _ := getFSKVStore(t, "")

	// Get on non existent key
	_, err := fskv.Get(testKey)
	assert.Error(t, err, "failed to error on non existent key")

	// Creating new key with Put
	assert.NoError(t, fskv.Put(testKey, testValue1), "failed to put key")

	// Get on existent key
	v, err := fskv.Get(testKey)
	assert.NoError(t, err, "errored getting value for new key")
	assert.Equal(t, testValue1, v)

	// Update value with second Put
	assert.NoError(t, fskv.Put(testKey, testValue2), "failed to put key")

	// Check that Get now returns updated value
	v, err = fskv.Get(testKey)
	assert.NoError(t, err, "errored getting new value for new key")
	assert.Equal(t, testValue2, v)
}

func TestFSKVStore_Persistence(t *testing.T) {
	fskv, dir := getFSKVStore(t, "")

	// Creating new key
	assert.NoError(t, fskv.Put(testKey, testValue1), "failed to put key")

	fskv2, _ := getFSKVStore(t, dir)

	// Check Get returns new value for key
	v, err := fskv2.Get(testKey)
	assert.NoError(t, err, "error getting persisted value for key")
	assert.Equal(t, testValue1, v)
}

func getFSKVStore(t *testing.T, dir string) (KVStore, string) {
	if len(dir) == 0 {
		path, err := ioutil.TempDir("", "kvstore-test")
		if err != nil {
			t.Fatal(err)
		}
		dir = path
	}
	fskv, err := NewFSKVStore(dir)
	if err != nil {
		t.Fatal(err)
	}
	return fskv, dir
}
