//! Quant1X Standard Library for Rust
//! 
//! This library provides cross-language compatible utilities for quantitative trading,
//! with implementations in C++, Go, and Rust that maintain API consistency.

pub mod timestamp;

// Re-export commonly used items
pub use timestamp::Timestamp;

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }

    #[test]
    fn test_timestamp_integration() {
        let ts = Timestamp::now();
        assert!(ts.value() > 0);
        
        let formatted = ts.to_string();
        assert!(!formatted.is_empty());
    }
}
