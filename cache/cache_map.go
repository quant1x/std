package cache

import "sync"

// Map 二次封装的泛型sync.Map
type Map[K any, V any] struct {
	syncMap sync.Map
}

func (m *Map[K, V]) Get(k K) (V, bool) {
	obj, ok := m.syncMap.Load(k)
	var v V
	if ok {
		v, ok = obj.(V)
	}
	return v, ok
}

func (m *Map[K, V]) Put(k K, v V) {
	m.syncMap.Store(k, v)
}
