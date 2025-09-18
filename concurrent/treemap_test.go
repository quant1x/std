package concurrent

import (
	"fmt"
	"math/rand"
	"strconv"
	"testing"
)

func TestTreeMap(t *testing.T) {
	treemap := NewTreeMap[string, int]()
	treemap.Put("a", 10)
	treemap.Put("1a", 12)
	v, ok := treemap.Get("a")
	fmt.Println(v, ok)
	v, ok = treemap.Get("a1")
	fmt.Println(v, ok)

	treemap.Each(func(key string, value int) {
		fmt.Println(key, value)
	})
}

func BenchmarkTreeMapWrite(b *testing.B) {
	treemap := NewTreeMap[string, int]()
	for i := 0; i < b.N; i++ {
		key := strconv.Itoa(i)
		value := i
		treemap.Put(key, value)
	}
}

func BenchmarkTreeMapRead(b *testing.B) {
	testTreemap := NewTreeMap[string, int]()
	testCount := 10000
	for i := 0; i < testCount; i++ {
		key := strconv.Itoa(i)
		value := rand.Int()
		testTreemap.Put(key, value)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		key := strconv.Itoa(i)
		_, _ = testTreemap.Get(key)
	}
}
