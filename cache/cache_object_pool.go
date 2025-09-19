package cache

import "sync"

// Pool 二次封装的泛型sync.Pool
type Pool[E any] struct {
	once sync.Once // 初始化sync.Pool的New接口
	pool sync.Pool // sync.Pool
	zero E         // 零值
}

// 初始化sync.Pool.New
func (p *Pool[E]) init() {
	p.pool = sync.Pool{New: func() any {
		var e E
		return &e
	}}
}

// Acquire 申请内存
func (p *Pool[E]) Acquire() *E {
	p.once.Do(p.init)
	obj := p.pool.Get().(*E)
	*obj = p.zero
	return obj
}

// Release 释放内存
func (p *Pool[E]) Release(obj *E) {
	p.once.Do(p.init)
	if obj == nil {
		return
	}
	p.pool.Put(obj)
}
