/*
 * Copyright (c) 2013 Hideya Ochiai, the University of Tokyo,  All rights reserved.
 *
 * Permission of redistribution and use in source and binary forms,
 * with or without modification, are granted, free of charge, to any person
 * obtaining the copy of this software under the following conditions:
 *
 *  1. Any copies of this source code must include the above copyright notice,
 *  this permission notice and the following statement without modification
 *  except possible additions of other copyright notices.
 *
 *  2. Redistributions of the binary code must involve the copy of the above
 *  copyright notice, this permission notice and the following statement
 *  in documents and/or materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * ieee1888_client.h
 *
 *  Created on: Oct 3, 2017
 *      Author: anolp
 *      License: Copyright@2017 EmOne
 */

#ifndef _IEEE1888_CLIENT_H_
#define _IEEE1888_CLIENT_H_

#define IEEE1888_FETCH_SUCCESS 1
#define IEEE1888_FETCH_FAIL 0

#define IEEE1888_WRITE_SUCCESS 1
#define IEEE1888_WRITE_FAIL 0

void ieee1888_client_init(void);
int ieee1888_client_fetch_from_server(void);
int ieee1888_client_write_to_server(void);

#endif /* _IEEE1888_CLIENT_H_ */
