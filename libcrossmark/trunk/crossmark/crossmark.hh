/* Crossmark processing library 
 * Copyright (C) 2006, Robert Staudinger <robert.staudinger@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*!
 * \file crossmark.hh
 * \brief Crossmark main include.
 */

#ifndef CROSSMARK_HH
#define CROSSMARK_HH

#include <crossmark/cm-features.hh>
#include <crossmark/cm-sink.hh>
#include <crossmark/cm-source.hh>

/*!
 * \brief Toplevel crossmark namespace. 
 * 
 * \todo Maybe use glib::ustring instead of std::string and glib::TimeVal?
 * \todo Pass errors to the Document, return them from Source::sputter() or even both?
 * \todo Not quite happy with citations, e.g. the sample from the v4 spec. Since crossmark
	 parsers must not handle macros parsing would be up to the application. Or does
	 that not hold for builtin macros?
 * \todo Helper functions
	 Timestamp * parseTimestamp (std::string &time); // or find a widely used c++ time class
 * \author Robert Staudinger <robert.staudinger@gmail.com>
 */
namespace crossmark {

/* placeholder for docs */

};  // namespace crossmark

#endif /* CROSSMARK_HH */
