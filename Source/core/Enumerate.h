#ifndef __ENUMERATE_H
#define __ENUMERATE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Module.h"
#include "Portability.h"
#include "Number.h"
#include "Optional.h"
#include "TextFragment.h"

namespace WPEFramework {
	namespace Core {
		// ---- Referenced classes and types ----

		// ---- Helper types and constants ----
#define ENUM_CONVERSION_BEGIN(ENUMERATE) namespace Core { template <>							\
const EnumerateConversion<ENUMERATE>* EnumerateType<ENUMERATE>::Table(const uint16_t index) {				\
	static WPEFramework::Core::EnumerateConversion<ENUMERATE> table[] = {

#define ENUM_CONVERSION_END(ENUMERATE)											\
		{ static_cast<ENUMERATE>(~0), nullptr, 0 } };								\
		return (index < ((sizeof(table)/sizeof(EnumerateType<ENUMERATE>))-1) ? &table[index] : nullptr);	\
		} }													

    // ---- Helper functions ----

    // ---- Class Definition ----
    template <typename ENUMERATEFIELD>
    struct EnumerateConversion {
        ENUMERATEFIELD value;
        const TCHAR* name;
        uint32_t length;
    };

    template <typename ENUMERATE>
    class EnumerateType {
    private:
        typedef struct Core::EnumerateConversion<ENUMERATE> BaseInformation;

    public:
        EnumerateType()
            : m_Value()
        {
        }
        explicit EnumerateType(const ENUMERATE Value)
            : m_Value(Value)
        {
        }
        explicit EnumerateType(const TCHAR value[], const bool caseSensitive = true)
            : m_Value()
        {
            if (caseSensitive) {
                operator=<true>(value);
            }
            else {
                operator=<false>(value);
            }
        }
        explicit EnumerateType(const Core::TextFragment& value, const bool caseSensitive = true)
            : m_Value()
        {
            if (caseSensitive) {
                operator=<true>(value);
            }
            else {
                operator=<false>(value);
            }
        }
        explicit EnumerateType(const EnumerateType<ENUMERATE>& copy)
            : m_Value(copy.m_Value)
        {
        }
        explicit EnumerateType(const uint32_t Value)
            : m_Value()
        {
            operator=(Value);
        }

        virtual ~EnumerateType()
        {
        }

        EnumerateType<ENUMERATE>& operator=(const EnumerateType<ENUMERATE>& RHS)
        {
            m_Value = RHS.m_Value;

            return (*this);
        }

        EnumerateType<ENUMERATE>& operator=(const ENUMERATE Value)
        {
            m_Value = Value;

            return (*this);
        }

        EnumerateType<ENUMERATE>& operator=(const uint32_t Value)
        {
            const EnumerateConversion<ENUMERATE>* pEntry = Find(Value);

            if (pEntry == nullptr) {
                m_Value = OptionalType<ENUMERATE>();
            }
            else {
                m_Value = pEntry->value;
            }

            return (*this);
        }

        template <bool CASESENSITIVE>
        EnumerateType<ENUMERATE>& operator=(const Core::TextFragment& text)
        {
            const EnumerateConversion<ENUMERATE>* pEntry = Find<CASESENSITIVE>(text);

            if (pEntry == nullptr) {
                m_Value = OptionalType<ENUMERATE>();
            }
            else {
                m_Value = pEntry->value;
            }
            return (*this);
        }

        template <bool CASESENSITIVE>
        EnumerateType<ENUMERATE>& operator=(const TCHAR text[])
        {
            const EnumerateConversion<ENUMERATE>* pEntry = Find<CASESENSITIVE>(text);

            if (pEntry == nullptr) {
                m_Value = OptionalType<ENUMERATE>();
            }
            else {
                m_Value = pEntry->value;
            }
            return (*this);
        }

        EnumerateType<ENUMERATE>& Assignment(bool caseSensitive, const TCHAR value[])
        {
            const EnumerateConversion<ENUMERATE>* pEntry;

            if (caseSensitive == true) {
                pEntry = Find<true>(value);
            }
            else {
                pEntry = Find<false>(value);
            }

            if (pEntry == nullptr) {
                m_Value = OptionalType<ENUMERATE>();
            }
            else {
                m_Value = pEntry->value;
            }
            return (*this);
        }

        inline bool IsSet() const
        {
            return (m_Value.IsSet());
        }

        inline void Clear()
        {
            m_Value.Clear();
        }

        inline ENUMERATE Value() const
        {
            return (m_Value.Value());
        }

        inline const TCHAR* Data() const
        {
            return (operator const TCHAR*());
        }

        operator const TCHAR*() const
        {
            const EnumerateConversion<ENUMERATE>* runner = nullptr;

            if (IsSet()) {
                runner = Find(m_Value.Value());
            }

            return ((runner == nullptr) ? _T("") : runner->name);
        }

    public:
        inline bool operator==(const EnumerateType<ENUMERATE>& rhs) const
        {
            return (m_Value == rhs.m_Value);
        }
        inline bool operator!=(const EnumerateType<ENUMERATE>& rhs) const
        {
            return (!operator==(rhs));
        }
        inline bool operator==(const ENUMERATE rhs) const
        {
            return (m_Value.IsSet()) && (m_Value.Value() == rhs);
        }
        inline bool operator!=(const ENUMERATE rhs) const
        {
            return (!operator==(rhs));
        }
        inline static const EnumerateConversion<ENUMERATE>* Entry(const uint16_t index)
        {
            return (Table(index));
        }

    private:
        OptionalType<ENUMERATE> m_Value;

		// Attach a table to this global parameter to get string conversions
		static const EnumerateConversion<ENUMERATE>* Table(const uint16_t index);

		template <bool CASESENSITIVE>
        const EnumerateConversion<ENUMERATE>* Find(const Core::TextFragment& value) const
        {
            const struct EnumerateConversion<ENUMERATE>* runner = Table(0);

            if (CASESENSITIVE == true) {
                while ((runner->name != nullptr) && (value != Core::TextFragment(runner->name, 0, runner->length))) {
                    runner++;
                }
            }
            else {
                while ((runner->name != nullptr) && (value.EqualText(Core::TextFragment(runner->name, runner->length)) == false)) {
                    runner++;
                }
            }

            return ((runner->name != nullptr) ? runner : nullptr);
        }

        template <bool CASESENSITIVE>
        const EnumerateConversion<ENUMERATE>* Find(const TCHAR value[]) const
        {
            const struct EnumerateConversion<ENUMERATE>* runner = Table(0);

            if (CASESENSITIVE == true) {
                while ((runner->name != nullptr) && (_tcscmp(runner->name, value) != 0)) {
                    runner++;
                }
            }
            else {
                while ((runner->name != nullptr) && (_tcsicmp(runner->name, value) != 0)) {
                    runner++;
                }
            }

            return ((runner->name != nullptr) ? runner : nullptr);
        }

        const EnumerateConversion<ENUMERATE>* Find(const uint32_t value) const
        {
            const struct EnumerateConversion<ENUMERATE>* runner = Table(0);

            while ((runner->name != nullptr) && (runner->value != static_cast<ENUMERATE>(value))) {
                runner++;
            }

            return ((runner->name != nullptr) ? runner : nullptr);
        }
    };
}
} // namespace Core

#endif // __ENUMERATE_H