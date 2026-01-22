/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@42tokyo.student.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 11:56:40 by hshimizu          #+#    #+#             */
/*   Updated: 2026/01/22 11:56:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memento.hpp"

Memento::Snapshot Memento::save() const {
    Snapshot snapshot;
    _saveToSnapshot(snapshot);
    return snapshot;
}

void Memento::load(Snapshot &snapshot) {
    _loadFromSnapshot(snapshot);
}
