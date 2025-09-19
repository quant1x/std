package std

type ObjectPool[T any] interface {
	Acquire() *T
	Release(obj *T)
}
