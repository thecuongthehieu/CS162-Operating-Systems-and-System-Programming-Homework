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
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"path/filepath"
)

// A Key Value store that only supports GET and PUT
type KVStore interface {
	Get(key string) (string, error)
	Put(key, value string) error
}

// FSKVStore implements the KVStore interface and is built on a filesystem
type FSKVStore struct {
	dir   string
	cache map[string]string
}

// Type check
var _ KVStore = FSKVStore{}

// NewFSKVStore takes the path to a directory and returns a FSKVStore.
// If a FSKVStore already exists there, it reads in the existing store.
func NewFSKVStore(path string) (KVStore, error) {
	err := os.MkdirAll(path, os.ModePerm)
	if err != nil {
		return nil, fmt.Errorf("failed to make directory: %v", err)
	}
	kvStore := FSKVStore{
		dir:   path,
		cache: make(map[string]string),
	}
	err = filepath.Walk(path, func(entryPath string, info os.FileInfo, err error) error {
		if err != nil {
			return fmt.Errorf("failed to walk directory: %v", err)
		}
		if info.IsDir() || info.Name() == path {
			return nil
		}
		contents, err := ioutil.ReadFile(entryPath)
		if err != nil {
			return fmt.Errorf("error reading KV entry %s: %s", entryPath, err)
		}
		kvStore.cache[info.Name()] = string(contents)
		return nil
	})
	if err != nil {
		return nil, err
	}
	return kvStore, nil
}

// Get returns the value for a given key or an error if one doesnt exist.
func (kv FSKVStore) Get(key string) (string, error) {
	value, exists := kv.cache[key]
	if !exists {
		return "", fmt.Errorf("tried to get entry that did not exist: %s", key)
	}
	return value, nil
}

// Put sets the value for key, creating the key if it doesn't exist.
// Then it flushes the entry to disk.
func (kv FSKVStore) Put(key, value string) error {
	if len(key) == 0 {
		return fmt.Errorf("key cannot be length 0")
	}
	kv.cache[key] = value
	return ioutil.WriteFile(path.Join(kv.dir, key), []byte(value), 0766)
}
