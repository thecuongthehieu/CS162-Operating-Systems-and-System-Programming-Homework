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
	"bufio"
	"fmt"
	"os"
	"strings"

	tpc_pb "github.com/Berkeley-CS162/tpc/pkg/rpc"
)

// Journal is the interface for a generic journaling utility.
type Journal interface {
	Size() int
	Empty()
	Append(e Entry) error
	NewIterator() *EntryIterator
}

// FileJournal is a implementation of the Journal interface backed by a file.
type FileJournal struct {
	size    int
	journal *os.File
	start   *journalEntry
	head    *journalEntry
}

// Entry is an export struct that represents one journal entry.
type Entry struct {
	Key    string
	Value  string
	Action tpc_pb.Action
}

// journalEntry is a wrapper around Entry and is used by the FileJournal.
// It adds a pointer to the next journal entry, which makes iterating the
// journal convenient and in memory.
type journalEntry struct {
	next  *journalEntry
	entry Entry
}

// Type check
var _ Journal = &FileJournal{}

// NewFileJournal takes a path and returns a FileJournal based on the file at that path.
// If the file already exists, it reads in the existing file.
func NewFileJournal(path string) (Journal, error) {
	f, err := os.OpenFile(path, os.O_RDWR|os.O_CREATE, 0766)
	if err != nil {
		return nil, err
	}
	sentinel := &journalEntry{}
	journal := &FileJournal{
		size:    0,
		journal: f,
		start:   sentinel,
		head:    sentinel,
	}
	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) != 3 {
			return nil, fmt.Errorf("each journal entry must have 3 fields: %v", fields)
		}
		journal.head.next = &journalEntry{
			entry: Entry{
				Key:    fields[0],
				Value:  fields[1],
				Action: tpc_pb.Action(tpc_pb.Action_value[fields[2]]),
			},
			next: nil,
		}
		journal.head = journal.head.next
		journal.size += 1
	}
	if err := scanner.Err(); err != nil {
		f.Close()
		return nil, err
	}
	return journal, nil
}

// Size returns the number of entries in a journal
func (j *FileJournal) Size() int {
	return j.size
}

// Empty completely empties the journal
func (j *FileJournal) Empty() {
	j.journal.Truncate(0)
	j.journal.Seek(0, 0)
	sentinel := &journalEntry{}
	j.head = sentinel
	j.start = sentinel
	j.size = 0
}

// Append takes an Entry and adds it to the journal. It performs
// linked list operations and writes the new head and the previous
// head to disk.
func (j *FileJournal) Append(e Entry) error {
	toWrite := fmt.Sprintf("%s %s %s\n", e.Key, e.Value, e.Action.String())
	numWritten, err := j.journal.WriteString(toWrite)
	if err != nil {
		return fmt.Errorf("error writing to journal: %v", err)
	} else if numWritten != len(toWrite) {
		return fmt.Errorf("expected to write %d chars but only wrote %d", len(toWrite), numWritten)
	}
	err = j.journal.Sync()
	if err != nil {
		return fmt.Errorf("error flushing journal: %v", err)
	}

	j.head.next = &journalEntry{
		next:  nil,
		entry: e,
	}
	j.head = j.head.next
	j.size += 1

	return nil
}

// NewIterator returns an EntryIterator for the journal.
func (l *FileJournal) NewIterator() *EntryIterator {
	return &EntryIterator{
		cur: l.start,
	}
}

// An EntryIterator is an iterator for iterating through the entries of
// a journal.
type EntryIterator struct {
	cur *journalEntry
}

// HasNext returns if there is another entry in the journal.
func (ei *EntryIterator) HasNext() bool {
	return ei.cur.next != nil
}

// Next returns the next entry in the journal, or an error if one does not exist.
func (ei *EntryIterator) Next() (Entry, error) {
	if !ei.HasNext() {
		return Entry{}, fmt.Errorf("no more entries in the iterator")
	}
	ei.cur = ei.cur.next
	return ei.cur.entry, nil
}
