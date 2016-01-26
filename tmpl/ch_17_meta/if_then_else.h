#pragma once

// Шаблон статического ветвления IfThenElse,
// возвращающий тип в зависимости от условия

// Первичный шаблон - не имеет определения
template <bool B, typename Tr, typename Fls>
struct IfThenElse;

// Частичная специализация для True
template <typename Tr, typename Fls>
struct IfThenElse<true, Tr, Fls>{
	typedef Tr Result;
};

// Частичная специализация для False
template <typename Tr, typename Fls>
struct IfThenElse<false, Tr, Fls>{
	typedef Fls Result;
};

