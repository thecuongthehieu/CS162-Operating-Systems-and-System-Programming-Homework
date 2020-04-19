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

package journal

import (
	"io/ioutil"
	"testing"

	pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
	"github.com/stretchr/testify/assert"
)

const (
	testKey     = "key"
	testValue   = "value"
	testAction1 = pb.Action_PREPARE
	testAction2 = pb.Action_COMMIT
)

func TestJournalEmpty(t *testing.T) {
	testFile, err := ioutil.TempFile("", "")
	defer testFile.Close()
	assert.NoError(t, err, "error making temp directory")
	entry1 := Entry{
		Key:    testKey,
		Value:  testValue,
		Action: testAction1,
	}
	entry2 := Entry{
		Key:    testKey,
		Value:  testValue,
		Action: testAction2,
	}

	// Test that journal creation and entry appending does not error
	journal, err := NewFileJournal(testFile.Name())
	assert.NoError(t, err, "failed to create journal from test file")
	assert.NoError(t, journal.Append(entry1), "failed to append entry 1")
	assert.NoError(t, journal.Append(entry2), "failed to append entry 2")

	// Test that the journal is emptied in memory
	journal.Empty()
	assert.Equal(t, journal.Size(), 0)
	journalIterator := journal.NewIterator()
	assert.Equal(t, journalIterator.HasNext(), false)

	// Test that the actual underlying log file is also emptied
	persistJournal, err := NewFileJournal(testFile.Name())
	assert.Equal(t, persistJournal.Size(), 0)
	persistJournalIterator := persistJournal.NewIterator()
	assert.Equal(t, persistJournalIterator.HasNext(), false)
}

func TestJournalPersistence(t *testing.T) {
	testFile, err := ioutil.TempFile("", "")
	defer testFile.Close()
	assert.NoError(t, err, "error making temp directory")
	entry1 := Entry{
		Key:    testKey,
		Value:  testValue,
		Action: testAction1,
	}
	entry2 := Entry{
		Key:    testKey,
		Value:  testValue,
		Action: testAction2,
	}

	// Test that journal creation and entry appending does not error
	journal, err := NewFileJournal(testFile.Name())
	assert.NoError(t, err, "failed to create journal from test file")
	assert.NoError(t, journal.Append(entry1), "failed to append entry 1")
	assert.NoError(t, journal.Append(entry2), "failed to append entry 2")

	// Test that new journal from dir has same journal entries
	persistJournal, err := NewFileJournal(testFile.Name())
	assert.NoError(t, err, "failed to persistence journal from test directory")

	journalIterator := persistJournal.NewIterator()

	entry1Retrieved, err := journalIterator.Next()
	assert.NoError(t, err, "failed to iterate first entry")
	assert.Equal(t, entry1, entry1Retrieved)

	entry2Retrieved, err := journalIterator.Next()
	assert.NoError(t, err, "failed to iterate second entry")
	assert.Equal(t, entry2, entry2Retrieved)
}
